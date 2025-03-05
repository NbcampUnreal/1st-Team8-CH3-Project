#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPatrolPos();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnyWhere)
	float FindTargetRadius = 5000.0f;

	UPROPERTY(EditAnyWhere)
	float MaxAngle = 60.0f;
};
