#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_AlertState.generated.h"

UCLASS()
class PROJECT_GGF_API UUBTTask_AlertState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UUBTTask_AlertState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override; // task 진행되는 동안 매 틱마다 호출
	virtual uint16 GetInstanceMemorySize() const override; // 경과 시간 저장

protected:
	UPROPERTY(EditAnywhere, Category = "Alert")
	float LostSightDelay = 2.0f;

private:
	bool bPlayerInSight = false;

};
