#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateTime.generated.h"

UCLASS()
class PROJECT_GGF_API UBTService_UpdateTime : public UBTService
{
	GENERATED_BODY()

public:
    UBTService_UpdateTime();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
