#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnumKeyType.h"
#include "AIStateManager.generated.h"

class AAIControllerCustom; 

UCLASS()
class PROJECT_GGF_API UAIStateManager : public UObject
{
	GENERATED_BODY()

public:
    void Initialize(UBlackboardComponent* Blackboard, AAIControllerCustom* Controller);
    void ChangeState(ELookState FromState, ELookState ToState);
    ELookState GetCurrentState() const;

private:
    UPROPERTY()
    AAIControllerCustom* AIController;

    UPROPERTY()
    UBlackboardComponent* BlackboardComp;

    ELookState CurrentState;

    bool CanTransitionToState(ELookState FromState, ELookState ToState);
    void ResetBlackboardStates();
    void ValidateAIState();
    void SetBlackboardState(ELookState NewState);
    void ForceInvestigate();

public:
    void HandlePlayerLost();
    void UpdateLookState(ELookState NewState);
};
