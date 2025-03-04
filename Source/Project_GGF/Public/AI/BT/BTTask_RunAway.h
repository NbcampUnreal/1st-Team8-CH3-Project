#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RunAway.generated.h"


UCLASS()
class PROJECT_GGF_API UBTTask_RunAway : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RunAway();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category="RunAway")
	FBlackboardKeySelector RunAwayLocationKey;
	
	UPROPERTY(EditAnywhere, Category="RunAway")
	float RunDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category="RunAway")
	float MaxWalkSpeedRun = 120.0f;

	UPROPERTY(EditAnywhere, Category="RunAway")
	float MaxWalkSpeedDefault = 60.0f;
};
