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
	if (nullptr == ControllingPawn)
		return false;

	// 후에 경계 코드 받으면 TargetKey Set하는 내용 넣기
	/*
	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGGFAIController::TargetKey));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;*/
	return false;
}
