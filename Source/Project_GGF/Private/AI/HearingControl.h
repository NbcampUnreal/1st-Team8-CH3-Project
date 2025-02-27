#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumKeyType.h"
#include "Perception/AIPerceptionComponent.h"
#include "HearingControl.generated.h"

class AAIControllerCustom;
class UBlackboardComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHearingControl : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHearingControl();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "AI|Hearing")
    float MinNoiseThreshold = 3.0f; // 이 값 이상의 소리만 처리

private:

    UPROPERTY()
    AAIControllerCustom* OwnerController;
    UPROPERTY()
    UBlackboardComponent* OwnerBlackboard;
    UPROPERTY()
    UAIPerceptionComponent* OwnerPerception;


    // 청각 이벤트 핸들러
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    // 소리 소스 분류
    ENoiseSourceType ClassifyNoiseSource(AActor* Source) const;

    // 소리 강도 계산
    float CalculateNoiseStrength(const FAIStimulus& Stimulus) const;

};
