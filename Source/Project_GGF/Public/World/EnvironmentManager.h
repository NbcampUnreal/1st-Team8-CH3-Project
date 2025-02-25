// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FEnvironmentTimeData.h"
#include "GameFramework/Actor.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "EnvironmentManager.generated.h"

UCLASS()
class PROJECT_GGF_API AEnvironmentManager : public AActor
{
	GENERATED_BODY()
	
	//——————————————————————————————————————————————————————————————————————
	// 생성자 & 소멸자
	//——————————————————————————————————————————————————————————————————————
	AEnvironmentManager();

private:
	//——————————————————————————————————————————————————————————————————————
	// 프로퍼티 & 변수
	//——————————————————————————————————————————————————————————————————————
	// Non-reflected

	// Rule
	// UPROPERTY(VisibleAnywhere)
	// float CurrentTime = 0.0f;

	// Time
	UPROPERTY(VisibleAnywhere, Category = "Time")
	float CurrentTime = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Time")
	float RealElapsedTime = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Time")
	float TimeDivision = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Time")
	bool bIsInitialStart = true;
	
	// Transition Index
	UPROPERTY(VisibleAnywhere, Category = "Index")
	bool bExistTransitionData = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 TotalIndex = -1;
	
	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 PreviousIndex = -1;

	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 CurrentIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 NextIndex = 1;

	
	// Transition
	UPROPERTY(VisibleAnywhere, Category = "Transition")
	bool bIsTransitioning = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Transition")
	float TransitionProgress = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Transition")
	float TransitionDuration = 3.0f;
	
	// Timer
	UPROPERTY(VisibleAnywhere)
	FTimerHandle TimeUpdateTimer;

	// Sun Rotation


	//——————————————————————————————————————————————————————————————————————
	// 함수
	//——————————————————————————————————————————————————————————————————————
	// 오버라이드
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	// Time
	UFUNCTION()
	void TimeUpdate(float DeltaTime);

	void TimeCheck();

	
	// Transition
	// 트랜지션은 시작한 후, 틱 마다 특정 시간 동안 0->1값으로 점진적으로 다음 인덱스로 보간된다.
	UFUNCTION()
	void StartTransition();
	
	UFUNCTION()
	void TransitionUpdate(float DeltaTime);
	
	UFUNCTION()
	void StopTransition();
	
	
	// Debugging
	UFUNCTION(CallInEditor, Category="EnvironmentManager")
	void NextTransition();

	UFUNCTION(CallInEditor, Category="EnvironmentManager")
	void PreviousTransition();


	// Lerp
	UFUNCTION()
	void LerpLightRotation(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha);

	UFUNCTION()
	void LerpPostProcess(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha);

	UFUNCTION()
	void LerpFog(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha);

	
	// Initial
	UFUNCTION(CallInEditor, Category="EnvironmentManager")
	void Initialize();

	UFUNCTION()
	void InitializePostProcess();

	UFUNCTION()
	void InitializeTransitionIndex();

	UFUNCTION()
	void InitalizeTimeData();

	UFUNCTION()
	void InitializeEnvironmentDataTable();


	// Calculate
	UFUNCTION()
	float CalculateSunRotation(const float Time);
	
	UFUNCTION()
	int32 FindTimeDataIndexForTime(const float Time);
	
	// Setter
	UFUNCTION()
	void SetTransitionIndex(int32& Index, int32 NewIndex);

	UFUNCTION()
	void SetCurrentTime(float NewTime);

protected:
	//——————————————————————————————————————————————————————————————————————
	// 프로퍼티 & 변수
	//——————————————————————————————————————————————————————————————————————
	// Non-reflected


	// Rule
	/**
	 * @brief 게임의 한 시간을 실제 초 단위로 나타낸 규칙 값
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float SecondsPerGameHour  = 60.0f;

	// Time
	// TODO: 시작 시간 -> 끝 시간을 제한시켜 순회하는 맵의 키를 무시하는 형태로?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time", meta=(UIMin = "0.0", UIMax = "24.0"))
	float StartTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time", meta=(UIMin = "0.0", UIMax = "24.0"))
	float EndTime = 24.0f;
	
	// Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Light | Sun")
	UDirectionalLightComponent* Sun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Light | Moon")
	UDirectionalLightComponent* Moon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UVolumetricCloudComponent* Cloud;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	USkyLightComponent* SkyLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	USkyAtmosphereComponent* SkyAtmosphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UExponentialHeightFogComponent* Fog;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UStaticMeshComponent* SkyMesh;


	// PostProcessVolume
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UPostProcessComponent* PostProcess;

	// EnvironmentTimeData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData")
	TArray<FEnvironmentTimeData> EnvironmentTimeDataTable;

public:
	//——————————————————————————————————————————————————————————————————————
	// 함수
	//——————————————————————————————————————————————————————————————————————
	// 오버라이드

	// Debugging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
	int32 DebuggingIndex = 0;

	UFUNCTION(CallInEditor, Category="Debugging")
	void DebugTransitionIndex();


	//——————————————————————————————————————————————————————————————————————
	// Get & Set
	//——————————————————————————————————————————————————————————————————————
	// Getter

	
	// Setter

	
};
