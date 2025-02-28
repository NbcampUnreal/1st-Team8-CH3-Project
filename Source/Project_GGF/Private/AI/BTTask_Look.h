#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Look.generated.h"

UCLASS()
class UBTTask_Look : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_Look();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    float WaitDuration = 3.0f;

private:
    UPROPERTY()
    UBlackboardComponent* CurrentBlackboard = nullptr;

    UPROPERTY()
    UBehaviorTreeComponent* CurrentOwnerComp = nullptr;

    UPROPERTY()
    AAIController* CurrentAIController = nullptr;

    UPROPERTY()
    AActor* CurrentTarget = nullptr;

    void OnTargetLost(UBlackboardComponent* BlackboardComp);

};