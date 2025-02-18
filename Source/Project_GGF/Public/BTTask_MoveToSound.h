#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToSound.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_MoveToSound : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToSound();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
