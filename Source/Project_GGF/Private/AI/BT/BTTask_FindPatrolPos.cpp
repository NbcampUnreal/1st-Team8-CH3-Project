#include "AI/BT/BTTask_FindPatrolPos.h"
#include "NavigationSystem.h"
#include "AI/GGFAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AGGFAIController::HomePosKey);
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, FindTargetRadius, NextPatrol))
	{
		FVector CurrentLocation = ControllingPawn->GetActorLocation();
		FVector CurrentDirection = ControllingPawn->GetActorForwardVector();
		FVector TargetDirection = (NextPatrol.Location - CurrentLocation).GetSafeNormal();
		
		// CurrentDirection, TargetDirection 사이의 각도
		float AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CurrentDirection, TargetDirection)));
		FVector CrossProduct = FVector::CrossProduct(CurrentDirection, TargetDirection);
		
		// 항상 MaxAngle 이내에서 좌회전하도록
		if (CrossProduct.Z <= 0 || AngleBetween > MaxAngle)
		{
			FVector ClampedDirection = CurrentDirection.RotateAngleAxis(-MaxAngle, FVector::UpVector);
			NextPatrol.Location = CurrentLocation + ClampedDirection * FindTargetRadius;
		}
		
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AGGFAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
