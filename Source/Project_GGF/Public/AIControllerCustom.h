#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "AIControllerCustom.generated.h"

UENUM(BlueprintType)
enum class EAIPerceptionSense : uint8
{
	EPS_Sight UMETA(DisplayName = "Sight"),
	EPS_Hearing UMETA(DisplayName = "Hearing")
};

UCLASS()
class PROJECT_GGF_API AAIControllerCustom : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerCustom();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	class UAIPerceptionComponent* AIPerception;
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;

	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);

private:

	UPROPERTY()
	class UBehaviorTree* BTAsset;
	UPROPERTY()
	class UBlackboardData* BBAsset;

};
