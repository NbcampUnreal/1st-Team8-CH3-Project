#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LookAround.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_LookAround : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_LookAround();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float InitialYaw; 
	float LastYaw; 
	float RotationSpeed; 
	float AccumulatedYaw;
};
