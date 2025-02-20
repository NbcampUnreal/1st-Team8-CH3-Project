#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindPatrolPos.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTaskNode_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
