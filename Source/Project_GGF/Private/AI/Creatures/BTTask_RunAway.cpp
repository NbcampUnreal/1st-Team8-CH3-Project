#include "AI/Creatures/BTTask_RunAway.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllerCustom.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto AICharacter = OwnerComp.GetAIOwner()->GetPawn();
    if (!AICharacter) return EBTNodeResult::Failed;

    FVector CurrentLocation = AICharacter->GetActorLocation();
    FVector AttackerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("AttackerLocation"));
    // 도망 방향으로 거리만큼 이동할 목표위치 설정
    FVector RunDirection = (CurrentLocation - AttackerLocation).GetSafeNormal();
    FVector RunDestination = CurrentLocation + (RunDirection * 1000.0f);

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(AICharacter->GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    FNavLocation NextLocation;
    if (NavSystem->GetRandomPointInNavigableRadius(RunDestination, 500.0f, NextLocation))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, NextLocation.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
