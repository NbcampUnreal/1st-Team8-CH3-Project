#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DrinkWater.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_DrinkWater : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_DrinkWater();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	AAIController* AIController;
	class AAnimal* AICharacter;

	float OriginalDetectionRadius;
};
