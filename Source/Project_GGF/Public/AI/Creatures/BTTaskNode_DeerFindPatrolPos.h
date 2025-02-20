#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_DeerFindPatrolPos.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTaskNode_DeerFindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_DeerFindPatrolPos();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
