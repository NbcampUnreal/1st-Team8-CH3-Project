#include "BTTask_Patrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/AIControllerCustom.h"

UBTTask_Patrol::UBTTask_Patrol()
{
    NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CurrentOwnerComp = &OwnerComp;
    CurrentAIController = OwnerComp.GetAIOwner();
    CurrentBlackboard = OwnerComp.GetBlackboardComponent();

    if (!CurrentAIController || !CurrentBlackboard)
    {
        return EBTNodeResult::Failed;
    }

    CurrentBlackboard->SetValueAsBool(TEXT("bPatrolling"), true);

    AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(CurrentAIController);
    if (CustomAIController)
    {
        CustomAIController->UpdateLookState(ELookState::Patrolling);
    }


    // 새로운 순찰 지점 찾기
    if (!FindNewPatrolPoint())
    {
        UE_LOG(LogTemp, Warning, TEXT("순찰 지점을 찾을 수 없습니다."));
        return EBTNodeResult::Failed;
    }

    CurrentAIController->MoveToLocation(CurrentPatrolPoint, AcceptanceRadius);

    // 순찰 진행 상태 확인
    FTimerDelegate PatrolTimerDelegate;
    PatrolTimerDelegate.BindUObject(this, &UBTTask_Patrol::CheckPatrolStatus);
    CurrentAIController->GetWorldTimerManager().SetTimer(PatrolTimerHandle, PatrolTimerDelegate, CheckInterval, true);

    return EBTNodeResult::InProgress;
}

bool UBTTask_Patrol::FindNewPatrolPoint()
{
    if (!CurrentAIController)
        return false;

    APawn* ControlledPawn = CurrentAIController->GetPawn();
    if (!ControlledPawn)
        return false;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(CurrentAIController->GetWorld());
    if (!NavSys)
        return false;

    // 순찰 지점 검색을 위한 기준 위치 설정
    FVector Origin = ControlledPawn->GetActorLocation();
    AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(CurrentAIController);
    if (CustomAIController)
    {
        FVector HomePos = CustomAIController->GetHomePos();
        if (!HomePos.IsZero())
        {
            Origin = HomePos;
        }
    }

    FNavLocation RandomPoint;
    bool bFound = false;

    for (int32 i = 0; i < MaxSearchAttempts; i++)
    {
        if (NavSys->GetRandomReachablePointInRadius(Origin, PatrolRadius, RandomPoint))
        {
            float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), RandomPoint.Location);
            if (Distance >= MinDistance)
            {
                CurrentPatrolPoint = RandomPoint.Location;
                bFound = true;

                UE_LOG(LogTemp, Warning, TEXT("새 순찰 지점 설정: %s"), *CurrentPatrolPoint.ToString());
                break;
            }
        }
    }

    if (!bFound && NavSys->GetRandomPointInNavigableRadius(Origin, PatrolRadius, RandomPoint))
    {
        CurrentPatrolPoint = RandomPoint.Location;
        bFound = true;
    }

    return bFound;
}

void UBTTask_Patrol::CheckPatrolStatus()
{
    if (!CurrentAIController || !CurrentBlackboard || !CurrentOwnerComp)
        return;

    if (CurrentBlackboard->GetValueAsBool(TEXT("bPlayerInSight")))
    {
        CurrentAIController->GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    UPathFollowingComponent* PathFollowingComp = CurrentAIController->GetPathFollowingComponent();
    if (!PathFollowingComp)
        return;

    if (PathFollowingComp->GetStatus() == EPathFollowingStatus::Idle)
    {
        CurrentAIController->GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
    }
}