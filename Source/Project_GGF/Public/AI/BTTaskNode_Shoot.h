#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Shoot.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTaskNode_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_Shoot();
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
