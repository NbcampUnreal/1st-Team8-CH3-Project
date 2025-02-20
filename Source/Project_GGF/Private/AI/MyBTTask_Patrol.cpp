#include "MyBTTask_Patrol.h"
#include "AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UMyBTTask_Patrol::UMyBTTask_Patrol()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UMyBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation NextPatrol;
	FVector Origin = ControllingPawn->GetActorLocation();

	if (NavSystem->GetRandomReachablePointInRadius(Origin, 3000.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}