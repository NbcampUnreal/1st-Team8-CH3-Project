#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "AIHearingHandler.generated.h"

class AAIControllerCustom;
class UAISenseManager;
class UAIStateManager;
class UBlackboardComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GGF_API UAIHearingHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIHearingHandler();
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
    UAISenseConfig_Hearing* HearingConfig;

    // 청각 이벤트 핸들러
    UFUNCTION()
    void HearingPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
    /** 소리의 출처가 같은 종족인지 여부 */
    bool IsSameSpecies(AActor* Source) const;

    // 소리 강도 계산
    float CalculateNoiseStrength(const FAIStimulus& Stimulus) const;

    UPROPERTY(EditAnywhere, Category = "AI Hearing")
    float HearingRadius = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "AI|Hearing")
    float MinNoiseThreshold = 3.0f; // 이 값 이상의 소리만 처리
};
