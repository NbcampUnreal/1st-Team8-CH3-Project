#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Look.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_Look : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Look();

	UPROPERTY(EditAnywhere, Category = "Look")
	float LookDuration;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	float CurrentTime;
};
