#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnumKeyType.h" 
#include "AIControllerCustom.generated.h"

class UAISightHandler;
class UAIStateManager;
class UAISenseManager;

class AAICharacter;
UCLASS()
class PROJECT_GGF_API AAIControllerCustom : public AAIController
{
    GENERATED_BODY()
public:
    AAIControllerCustom();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;
    
private:
    UPROPERTY()
    UAISightHandler* SightHandler;

    UPROPERTY()
    UAIStateManager* StateManager;

    UPROPERTY()
    UAISenseManager* SenseManager;

    UPROPERTY()
    UBlackboardComponent* BlackboardComp;

public:
    UAISenseManager* GetSenseManager() const { return SenseManager; }
    UAIStateManager* GetStateManager() const { return StateManager; }
    UAISightHandler* GetSightHandler() const { return SightHandler; }

    void SetPatrolPos(const FVector& NewPatrolPos);
    FVector GetPatrolPos() const;
    void SetHomePos(const FVector& NewHomePos);
    FVector GetHomePos() const;

    static const FName PatrolPosKey;
    static const FName TargetKey;
    static const FName HomePosKey;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBlackboardData* BBAsset;
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBehaviorTree* BTAsset;


};