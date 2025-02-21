#include "Project_GGF/Public/Character/Data/StaminaComponent.h"
#include "GameFramework/Actor.h"


UStaminaComponent::UStaminaComponent()
{

    PrimaryComponentTick.bCanEverTick = false; 

	MaxStamina = 100.f;
	Stamina = MaxStamina;
	StaminaDrainRate = 10.0f;
	StaminaRegenRate = 5.0f;
}



void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UStaminaComponent::RestoreStamina()
{
	if (Stamina < MaxStamina)
	{
		Stamina = FMath::Clamp(Stamina + StaminaRegenRate, 0.0f, MaxStamina);
		FString StaminaText = FString::Printf(TEXT("Stamina: %.0f / %.0f"), Stamina, MaxStamina);
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, StaminaText);
	}
	else
	{
		StopStaminaRecovery();
	}
}

void UStaminaComponent::UseStamina()
{

	if (Stamina > 0)
	{
		Stamina = FMath::Clamp(Stamina- StaminaDrainRate, 0.0f, MaxStamina);
		FString StaminaText = FString::Printf(TEXT("Stamina: %.0f / %.0f"), Stamina, MaxStamina);
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, StaminaText);

	}
	else
	{
		return;
	}
}

void UStaminaComponent::StartStaminaRecovery()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(
			StaminaRegenTimer,
			this,
			&UStaminaComponent::RestoreStamina,
			2.0f,
			true
		);
	}

}


void UStaminaComponent::StopStaminaRecovery()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
}


