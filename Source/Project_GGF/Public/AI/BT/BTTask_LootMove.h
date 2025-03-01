#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LootMove.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_LootMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_LootMove();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AAICharacter* AICharacter;
	FVector LootLocation;
};
