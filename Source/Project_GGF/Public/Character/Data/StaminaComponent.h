#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"  
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GGF_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UStaminaComponent();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaBoostDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaBoostMultiplier;


	FTimerHandle StaminaRegenTimer;
	FTimerHandle StaminaBoostTimerHandle;


public:

	void RestoreStamina();
	void UseStamina();
	void StartStaminaRecovery();
	void StopStaminaRecovery();
	void ActivateStaminaBoost();
	void ResetStaminaBoost();

	float GetStamina() { return Stamina; }


};
