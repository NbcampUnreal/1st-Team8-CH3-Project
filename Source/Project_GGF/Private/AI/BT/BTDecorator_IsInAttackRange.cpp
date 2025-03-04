#include "AI/BT/BTDecorator_IsInAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/GGFAIController.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!ControllingPawn) return false;
	
	FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AGGFAIController::TargetKey);
	float DistanceToTarget = FVector::Dist(ControllingPawn->GetActorLocation(), TargetLocation);
	UE_LOG(LogTemp, Warning, TEXT("distance: %f"), DistanceToTarget);
	
	bResult = (DistanceToTarget <= AttackRange);
	return bResult;
}
