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
	static const FName TargetKey;

	UPROPERTY(VisibleAnywhere, Category = "AI Perception")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI Perception")
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditAnywhere)
	float Sight1Range = 3000.0f;
	
	UPROPERTY(EditAnywhere)
	float Sight2Range = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FGenericTeamId TeamId;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BTAsset;
	
	UPROPERTY(EditAnywhere)
	class UBlackboardData* BBAsset;

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	FTimerHandle InvestigateUpdateTimer;
	void UpdateTargetPos();
};
