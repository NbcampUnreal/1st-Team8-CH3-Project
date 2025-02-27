#include "BTTask_ReturnToHome.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "AI/AIControllerCustom.h"
#include "AI/Manager/AIStateManager.h"

/*
*  ai가 home위치로 돌아옴
*/

UBTTask_ReturnToHome::UBTTask_ReturnToHome()
{
    NodeName = TEXT("Return to Home");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ReturnToHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BlackboardComp)
        return EBTNodeResult::Failed;

    AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(AIController);
    if (!CustomAIController)
        return EBTNodeResult::Failed;

    FVector HomeLocation = CustomAIController->GetHomePos();

    EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(HomeLocation, -1.0f, true);

    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReturnToHome: 홈 위치로 이동 실패"));
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::InProgress;
}

/*
*  AI가 목표 위치에 도착했는지 확인하는 함수
*/

void UBTTask_ReturnToHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BlackboardComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent();
    if (!PathComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UE_LOG(LogTemp, Verbose, TEXT("ReturnToHome Tick: PathStatus=%d"), (int)PathComp->GetStatus());

    if (PathComp->GetStatus() == EPathFollowingStatus::Idle ||
        PathComp->GetStatus() == EPathFollowingStatus::Paused ||
        PathComp->GetStatus() == EPathFollowingStatus::Waiting)
    {
        BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), true);
        BlackboardComp->SetValueAsBool(TEXT("bReturnToHome"), false);
        BlackboardComp->SetValueAsBool(TEXT("bSearching"), false);
        BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), false);

        BlackboardComp->ClearValue(TEXT("LastKnownPos"));

        AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(AIController);
        if (CustomAIController->GetStateManager())
        {
            CustomAIController->GetStateManager()->UpdateLookState(ELookState::Patrolling);
            UE_LOG(LogTemp, Warning, TEXT("ReturnToHome: 완료, 패트롤 상태로 전환"));
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}