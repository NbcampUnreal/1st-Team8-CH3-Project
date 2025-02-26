#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LookAround.generated.h"

UCLASS()
class UBTTask_LookAround : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_LookAround();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "AI")
    float LookAroundDuration = 3.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float MaxRotationAngle = 180.0f;


    FRotator InitialRotation;
    FRotator TargetRotation;
    int32 RotationDirection;

    UPROPERTY()
    AAIController* CurrentAIController;

    UPROPERTY()
    UBehaviorTreeComponent* CurrentOwnerComp;
};