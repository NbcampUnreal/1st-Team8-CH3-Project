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
	static const FName TargetKey;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	class UAIPerceptionComponent* AIPerception;
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;

	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBlackboardData> BBAsset; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTree> BTAsset; 
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	//UAIPerceptionComponent* AIPerception;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	//UAISenseConfig_Sight* SightConfig;
	

private:

	void HandleLostSight();
	UPROPERTY()
	FTimerHandle LostSightTimerHandle;

};
