#include "AI/Creatures/BTTaskNode_DeerFindPatrolPos.h"
#include "AI/Creatures/DeerDoe.h"
#include "AI/AIControllerCustom.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_DeerFindPatrolPos::UBTTaskNode_DeerFindPatrolPos()
{
	NodeName = TEXT("DeerFindPatrolPos");
}

EBTNodeResult::Type UBTTaskNode_DeerFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADeerDoe* DeerCharacter = Cast<ADeerDoe>(OwnerComp.GetAIOwner()->GetPawn());
	if (!DeerCharacter) return EBTNodeResult::Failed;

	FVector HerdCenter = DeerCharacter->CalculateAveragePos();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(DeerCharacter->GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation NextPatrol;

	if (NavSystem->GetRandomReachablePointInRadius(HerdCenter, 2000.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
