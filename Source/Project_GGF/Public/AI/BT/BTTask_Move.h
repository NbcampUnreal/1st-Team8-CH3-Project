#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Move.generated.h"

UCLASS()
class PROJECT_GGF_API UBTTask_Move : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Move();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Move")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category="Move")
	float MoveSpeed = 120.0f;

	UPROPERTY(EditAnywhere, Category="Move")
	float OriginalSpeed = 0.0f;
};
