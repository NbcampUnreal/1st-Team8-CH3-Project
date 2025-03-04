#pragma once

#include "CoreMinimal.h"
#include "FEnvionmentPostProcess.h"
#include "FEnvironmentTimeData.generated.h"


USTRUCT(BlueprintType)
struct FEnvironmentTimeData
{
	GENERATED_BODY()

	// Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData", meta = (UIMin = "0.0", UIMax = "24.0"))
	int Priority = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData", meta = (UIMin = "0.0", UIMax = "24.0"))
	float Time = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData")
	float TimeRotationCoefficient = 0.0f;

	// Post-Process
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData")
	FEnvironmentPostProcess PostProcess;

	// Fog
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData")
	float FogDensity = 0.015f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData")
	float FogHeightFalloff = 0.2f;
};
