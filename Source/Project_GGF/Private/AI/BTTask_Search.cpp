#include "BTTask_Search.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/AIControllerCustom.h"
#include "AI/Manager/AIStateManager.h"

UBTTask_Search::UBTTask_Search()
{
    NodeName = TEXT("Search");
    bNotifyTick = false;
    bCreateNodeInstance = true;

    SearchRadius = 500.0f;
    SearchDuration = 10.0f; // 전체 수색 지속 시간
    MaxSearchAttempts = 3;

}

EBTNodeResult::Type UBTTask_Search::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BlackboardComp)
        return EBTNodeResult::Failed;

    AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(AIController);
    if (CustomAIController->GetStateManager())
    {
        CustomAIController->GetStateManager()->UpdateLookState(ELookState::Searching);
    }

    FVector LastKnownLocation = BlackboardComp->GetValueAsVector(TEXT("LastKnownPos"));

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AIController->GetWorld());
    if (!NavSys)
        return EBTNodeResult::Failed;

    TArray<FVector> SearchPoints;

    // 2~3회 랜덤 지점 찾기
    int32 SearchCount = FMath::RandRange(2, 3);
    for (int32 Attempts = 0; Attempts < SearchCount; Attempts++)
    {
        FNavLocation RandomSearchPoint;
        bool bFoundPoint = false;

        // 최대 시도 횟수 내에서 유효한 지점 찾기
        for (int32 FindAttempts = 0; FindAttempts < MaxSearchAttempts; FindAttempts++)
        {
            if (NavSys->GetRandomReachablePointInRadius(LastKnownLocation, SearchRadius, RandomSearchPoint))
            {
                float DistanceFromInitial = FVector::Dist(LastKnownLocation, RandomSearchPoint.Location);
                if (DistanceFromInitial > 200.0f)  // 최소 2미터 이상 떨어진 지점
                {
                    SearchPoints.Add(RandomSearchPoint.Location);
                    bFoundPoint = true;
                    break;
                }
            }
        }

        if (!bFoundPoint)
        {
            UE_LOG(LogTemp, Warning, TEXT("Search: Could not find valid search point"));
        }
    }

    // 수색 지점이 없으면 실패
    if (SearchPoints.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    // 첫 번째 수색 지점으로 이동
    EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(SearchPoints[0], -1.0f, true, true);

    UE_LOG(LogTemp, Warning, TEXT("Search: Moving to first location %s, Result: %d"),
        *SearchPoints[0].ToString(), (int32)MoveResult);

    // 클래스 멤버 변수 초기화
    mSearchIndex = 0;
    mSearchPoints = SearchPoints;
    mAIController = AIController;
    mBlackboardComp = BlackboardComp;
    mOwnerComp = &OwnerComp;

    // 일정 간격으로 탐색을 진행
    AIController->GetWorldTimerManager().SetTimer(
        mSearchTimerHandle,  
        this,
        &UBTTask_Search::SearchTimerCallback,
        5.0f,
        true
    );

    return EBTNodeResult::InProgress;
}

/*
* 일정 시간마다 탐색을 진행
*/

void UBTTask_Search::SearchTimerCallback()
{
    // 플레이어 발견 시 즉시 중단
    if (mBlackboardComp->GetValueAsBool(TEXT("bPlayerInSight")))
    {
        mAIController->GetWorldTimerManager().ClearTimer(mSearchTimerHandle);
        FinishLatentTask(*mOwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // 현재 지점에 도착했다면 다음 지점으로 이동
    mSearchIndex++;
    if (mSearchIndex < mSearchPoints.Num())
    {
        // 다음 위치로 이동
        mAIController->MoveToLocation(mSearchPoints[mSearchIndex], -1.0f, true, true);
        UE_LOG(LogTemp, Warning, TEXT("Search: Moving to next location %s"),
            *mSearchPoints[mSearchIndex].ToString());
    }
    else
    {
        // 모든 수색 지점 탐색 완료
        mAIController->GetWorldTimerManager().ClearTimer(mSearchTimerHandle);

        // 명확하게 블랙보드 상태 초기화
        mBlackboardComp->SetValueAsBool(TEXT("bSearching"), false);
        mBlackboardComp->SetValueAsBool(TEXT("bReturnToHome"), true);
        mBlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), false);
        mBlackboardComp->SetValueAsBool(TEXT("bInvestigate"), false);

        // 홈 위치로 복귀
        AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(mAIController);
        if (CustomAIController->GetStateManager())
        {
            CustomAIController->GetStateManager()->UpdateLookState(ELookState::ReturnToHome);
            UE_LOG(LogTemp, Warning, TEXT("Search: All points searched, returning home"));
        }

        FinishLatentTask(*mOwnerComp, EBTNodeResult::Succeeded);
    }
}