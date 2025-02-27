#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AISightHandler.generated.h"

class AAIControllerCustom;
class UAISenseManager;
class UBlackboardComponent;
class UAIStateManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GGF_API UAISightHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAISightHandler();
	void Initialize(AAIControllerCustom* InController, UAIPerceptionComponent* InPerception);


private:
	UPROPERTY()
	AAIControllerCustom* AIController;

	UPROPERTY()
	UAISenseManager* SenseManager;

	UPROPERTY()
	UAIStateManager* StateManager;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, Category = "AI Perception")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category = "AI Perception")
	UAISenseConfig_Sight* SightConfig;

	/** 감각 업데이트 이벤트 */
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	/** 특정 위치가 AI의 시야에 있는지 확인 */
	bool IsInSight(const FVector& TargetLocation) const;

	/** 각도를 무시하고 AI의 감지 범위 내에 있는지 확인 */
	bool IsInSightIgnoreAngle(const FVector& TargetLocation, bool bCheckAngle) const;

	void StartPlayerInSightTimer();;
	void OnPlayerInSightTimerFinished();
	FTimerHandle PlayerInSightTimerHandle;

public:
	UPROPERTY(EditAnywhere, Category = "AI Detection")
	float DetectionRadius = 1000.0f; // 감지 반경

	UPROPERTY(EditAnywhere, Category = "AI Detection")
	float MaxSightAngle = 60.0f; // ai 최대 시야각 

};
