#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AAnimal* AICharacter;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackMontage;
};
