#include "BTTaskNode_FindPatrolPos.h"
#include "AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AI/Creatures/DeerDoe.h"

UBTTaskNode_FindPatrolPos::UBTTaskNode_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTaskNode_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation NextPatrol;
	FVector Origin = ControllingPawn->GetActorLocation();

	// 사슴만 무리지어 이동하도록
	ADeerDoe* Deer = Cast<ADeerDoe>(ControllingPawn);
	if (Deer)
	{
		if (Deer->bIsLeader)
		{
			// 리더
			if (NavSystem->GetRandomReachablePointInRadius(Origin, 10000.0f, NextPatrol))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, NextPatrol.Location);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::LeaderPosKey, NextPatrol.Location);
				return EBTNodeResult::Succeeded;
			}
		}
		else
		{
			// 팔로워는 리더의 위치를 따라도록
			FVector LeaderPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIControllerCustom::LeaderPosKey);
			if (!LeaderPos.IsZero())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, LeaderPos);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	else
	{
		if (NavSystem->GetRandomReachablePointInRadius(Origin, 10000.0f, NextPatrol))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, NextPatrol.Location);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}