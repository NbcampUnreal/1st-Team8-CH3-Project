#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Stop.generated.h"


UCLASS()
class UBTTask_Stop : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_Stop();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};