#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GGFAIController.generated.h"

struct FAIStimulus;

UCLASS()
class PROJECT_GGF_API AGGFAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGGFAIController();

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName InvestigatePosKey;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BTAsset;
	
	UPROPERTY(EditAnywhere)
	class UBlackboardData* BBAsset;

	UPROPERTY(VisibleAnywhere, Category = "AI Perception")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI Perception")
	class UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	FTimerHandle InvestigateUpdateTimer;
	void UpdateInvestigatePos();
};
