#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_Patrol.generated.h"

UCLASS()
class UBTTask_Patrol : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_Patrol();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float PatrolRadius = 1500.0f;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float MinDistance = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float AcceptanceRadius = 150.0f;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    int32 MaxSearchAttempts = 5;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float CheckInterval = 0.5f;

private:
    FTimerHandle PatrolTimerHandle;

    UPROPERTY()
    UBehaviorTreeComponent* CurrentOwnerComp = nullptr;

    UPROPERTY()
    AAIController* CurrentAIController = nullptr;

    UPROPERTY()
    UBlackboardComponent* CurrentBlackboard = nullptr;

    UPROPERTY()
    FVector CurrentPatrolPoint = FVector::ZeroVector;

    UFUNCTION()
    void CheckPatrolStatus();

    bool FindNewPatrolPoint();
};