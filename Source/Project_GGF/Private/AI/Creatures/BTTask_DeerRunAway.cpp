#include "AI/Creatures/BTTask_DeerRunAway.h"
#include "AI/Creatures/DeerDoe.h"
#include "AI/AIControllerCustom.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_DeerRunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ADeerDoe* DeerCharacter = Cast<ADeerDoe>(OwnerComp.GetAIOwner()->GetPawn());
    if (!DeerCharacter) return EBTNodeResult::Failed;

    FVector CurrentLocation = DeerCharacter->GetActorLocation();
    FVector AttackerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("AttackerLocation"));

    // 무리의 중심점
    FVector HerdCenter = DeerCharacter->CalculateAveragePos();

    // attacker 기준으로 무리 중심 방향으로 
    FVector RunDirection = (HerdCenter - AttackerLocation).GetSafeNormal();
    FVector RunDestination = CurrentLocation + (RunDirection * 1000.0f);

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(DeerCharacter->GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    FNavLocation NextLocation;
    if (NavSystem->GetRandomPointInNavigableRadius(RunDestination, 500.0f, NextLocation))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, NextLocation.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
