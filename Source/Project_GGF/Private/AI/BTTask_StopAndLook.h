#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopAndLook.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_StopAndLook : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_StopAndLook();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
