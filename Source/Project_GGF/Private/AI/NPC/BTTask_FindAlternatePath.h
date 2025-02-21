#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindAlternatePath.generated.h"

UCLASS()
class UBTTask_FindAlternatePath : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindAlternatePath();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
