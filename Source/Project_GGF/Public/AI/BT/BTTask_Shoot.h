#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Shoot.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Shoot();
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AGGFAICharacter* AICharacter;
};
