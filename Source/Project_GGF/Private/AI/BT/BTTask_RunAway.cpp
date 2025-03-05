#include "AI/BT/BTTask_RunAway.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/GGFAIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_RunAway::UBTTask_RunAway()
{
    NodeName = TEXT("RunAway");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;
    
    ACharacter* AICharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    AICharacter->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedRun;

    FVector CurrentLocation = AICharacter->GetActorLocation();
    FVector DangerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(RunAwayLocationKey.SelectedKeyName);

    // 도망 방향으로 거리만큼 이동할 목표위치 설정
    FVector RunDirection = (CurrentLocation - DangerLocation).GetSafeNormal();
    FVector RunDestination = CurrentLocation + (RunDirection * RunDistance);

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(AICharacter->GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    FNavLocation NextLocation;
    if (NavSystem->GetRandomPointInNavigableRadius(RunDestination, 500.0f, NextLocation))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(AGGFAIController::PatrolPosKey, NextLocation.Location);
        
        EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(
            NextLocation.Location,
            100.0f,
            true,
            true,
            false,
            true);
        
        if (MoveResult == EPathFollowingRequestResult::Failed)
        {
            return EBTNodeResult::Failed;
        }
        else
        {
            return EBTNodeResult::InProgress;
        }
    }
    return EBTNodeResult::Failed;
}

void UBTTask_RunAway::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
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
    
    if (PathComp->DidMoveReachGoal())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

void UBTTask_RunAway::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    ACharacter* AICharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AICharacter)
    {
        AICharacter->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedDefault;
    }
}
