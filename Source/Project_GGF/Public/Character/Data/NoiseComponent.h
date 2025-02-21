#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"  
#include "NoiseComponent.generated.h"

USTRUCT(BlueprintType)
struct FGameNoise
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Intensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Radius;

	float Timestamp;

	// 기본 생성자
	FGameNoise()
		: Location(FVector::ZeroVector), Intensity(0.0f), Radius(0.0f)
	{
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GGF_API UNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNoiseComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Noise")
	int32 MaxNoiseHistory = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Noise")
	TArray<FGameNoise> NoiseHistory;

	float NoiseIntensity;
	float NoiseRadius;
	float NoiseDelay;
	float NoiseLifetime;

	FVector NoiseLocation;
	FTimerHandle NoiseTimerHandle;


protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	
	void GenerateNoise(FVector NoiseLocation, float Intensity, float Radius);
	void StartNoiseTimer(float Intensity, float Radius);
	void GenerateNoiseTimer();
	void StopNoiseTimer();
	void CleanupOldNoises();
		
};
