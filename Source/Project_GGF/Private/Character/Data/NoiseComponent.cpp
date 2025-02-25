#include "Project_GGF/Public/Character/Data/NoiseComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

UNoiseComponent::UNoiseComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	AverageIntensity = 50.0f;
	AverageRadiuse = 400.0f;
	NoiseIntensity = AverageIntensity;
	NoiseRadius = AverageRadiuse;
	NoiseDelay = 0.25f;
	NoiseLifetime = 3.0f;
}


void UNoiseComponent::BeginPlay()
{
	Super::BeginPlay();


	
}

void UNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CleanupOldNoises();  
}


void UNoiseComponent::GenerateNoise(FVector NewNoiseLocation, float Intensity, float Radius)
{

	FGameNoise  NewNoise;
	NewNoise.Location = NewNoiseLocation;
	NewNoise.Intensity = Intensity;
	NewNoise.Radius = Radius;
	NewNoise.Timestamp = GetWorld()->GetTimeSeconds();

	if (NoiseHistory.Num() >= MaxNoiseHistory)
	{
		NoiseHistory.RemoveAt(0);
	}

	NoiseHistory.Add(NewNoise);

}

void UNoiseComponent::StartNoiseTimer(float Intensity, float Radius)
{
	if (GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(NoiseTimerHandle))
	{
		NoiseIntensity = Intensity;
		NoiseRadius = Radius;

		
		GetWorld()->GetTimerManager().SetTimer(
			NoiseTimerHandle,
			this,
			&UNoiseComponent::GenerateNoiseTimer,
			NoiseDelay,
			true     
		);
	}
}

void UNoiseComponent::GenerateNoiseTimer()
{
	NoiseLocation = GetOwner()->GetActorLocation();
	GenerateNoise(NoiseLocation, NoiseIntensity, NoiseRadius);
}

void UNoiseComponent::StopNoiseTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(NoiseTimerHandle);
}

void UNoiseComponent::CleanupOldNoises()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	NoiseHistory.RemoveAll([this, CurrentTime](const FGameNoise& Noise)
		{
			return (CurrentTime - Noise.Timestamp) > NoiseLifetime;
		});
}