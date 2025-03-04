#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsLowHealth.generated.h"

UCLASS()
class PROJECT_GGF_API UBTDecorator_IsLowHealth : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere)
	int MaxHealth;

	UPROPERTY(EditAnywhere)
	int MinHealth;
};
