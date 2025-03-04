#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Investigate.generated.h"


UCLASS()
class PROJECT_GGF_API UBTTask_Investigate : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Investigate();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
