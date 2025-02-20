#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_Patrol.generated.h"

UCLASS()
class PROJECT_GGF_API UMyBTTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMyBTTask_Patrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
