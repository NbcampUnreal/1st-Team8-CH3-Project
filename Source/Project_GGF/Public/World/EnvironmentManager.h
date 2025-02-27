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
	
	// Time
	/**
	 * @brief 게임 내 표현되는 경과 시간 (예 : 24시)
	 */
	UPROPERTY(VisibleAnywhere, Category = "Time")
	float CurrentTime = 0.0f;
	
	/**
	 * @brief 게임 시작 후, 실제 경과 시간
	 */
	UPROPERTY(VisibleAnywhere, Category = "Time")
	float RealElapsedTime = 0.0f;
	
	/**
	 * @brief 환경 트랜지션 데이터 배열의 크기에 따른 시간 분기점
	 */
	UPROPERTY(VisibleAnywhere, Category = "Time")
	float TimeDivision = 0.0f;

	
	// Transition Index
	/**
	 * @brief 환경 트랜지션 데이터 배열이 존재하는 가 여부
	 */
	UPROPERTY(VisibleAnywhere, Category = "Index")
	bool bExistTransitionData = false;
	
	/**
	 * @brief 환경 트랜지션 데이터 배열의 크기
	 */
	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 TotalIndex = -1;
	
	/**
	 * @brief 트랜지션 이전 인덱스
	 */
	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 PreviousIndex = -1;

	/**
	 * @brief 트랜지션 현재 인덱스
	 */
	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 CurrentIndex = 0;

	/**
	 * @brief 트랜지션 다음 인덱스
	 */
	UPROPERTY(VisibleAnywhere, Category = "Index")
	int32 NextIndex = 1;

	
	// Transition
	/**
	 * @brief 트랜지션 중 일때 true, 아닐때 false
	 */
	UPROPERTY(VisibleAnywhere, Category = "Transition")
	bool bIsTransitioning = false;
	
	/**
	 * @brief 트랜지션 진행도 - 0.0f ~ 1.0f
	 */
	UPROPERTY(VisibleAnywhere, Category = "Transition")
	float TransitionProgress = 1.0f;

	/**
	 * @brief 트랜지션 진행 딜레이 및 기간
	 */
	UPROPERTY(EditAnywhere, Category = "Transition")
	float TransitionDuration = 3.0f;


	//——————————————————————————————————————————————————————————————————————
	// 함수
	//——————————————————————————————————————————————————————————————————————
	// 오버라이드
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 게임 시작 시, 초기화 Initialize() 실행
	 */
	virtual void BeginPlay() override;
	
	// Time
	
	/**
	 * @brief 세계 시간, 현실 시간에 대한 경과 업데이트
	 * @param DeltaTime Tick() -> 델타 타임
	 */
	UFUNCTION()
	void TimeUpdate(float DeltaTime);
	
	/**
	 * @brief 시간 경과와 트랜지션 데이터를 확인한 후, 트랜지션 실행
	 */
	UFUNCTION()
	void TimeCheck();

	
	// Transition
	// 트랜지션은 시작한 후, 틱 마다 특정 시간 동안 0->1값으로 점진적으로 다음 인덱스로 보간된다.
	/**
	 * @brief 트랜지션 시작
	 */
	UFUNCTION()
	void StartTransition();
	
	/**
	 * @brief 트랜지션 업데이트
	 * @param DeltaTime 델타 타임에 따라, 선형 보간
	 */
	UFUNCTION()
	void TransitionUpdate(float DeltaTime);
	
	/**
	 * @brief 트랜지션 정지 및 트랜지션 컨트롤 데이터 초기화
	 */
	UFUNCTION()
	void StopTransition();
	
	
	// Debugging
	/**
	 * @brief 다음 트랜지션
	 */
	UFUNCTION(CallInEditor, Category="EnvironmentManager")
	void NextTransition();

	// TODO: 이전 트랜지션 구현을 안했기 때문에, 필요
	/**
	 * @brief 이전 트랜지션
	 */
	UFUNCTION(CallInEditor, Category="EnvironmentManager")
	void PreviousTransition();

	/**
	 * @brief 인덱스에 따른 트랜지션 보기
	 * @param Index 트랜지션 인덱스
	 */
	UFUNCTION()
	void ViewTransition(int32 Index);

	
	// Lerp
	/**
	 * @brief 현재 라이트와 타겟 라이트와의 알파값에 따른 보간 이동
	 * @param Current 현재 환경 타임 데이터
	 * @param Target 타겟 환경 타임 데이터
	 * @param Alpha 보간 알파
	 */
	UFUNCTION()
	void LerpLightRotation(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha);

	/**
	 * @brief 현재 포스트 프로세스 세팅과 타겟 포스트 프로세스 세팅의 알파값에 따른 보간 변경
	* @param Current 현재 환경 타임 데이터
	 * @param Target 타겟 환경 타임 데이터
	 * @param Alpha 보간 알파
	 */
	UFUNCTION()
	void LerpPostProcess(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha);

	/**
	 * @brief 현재 포그와 타겟 포그의 알파값에 따른 보간 변경
	* @param Current 현재 환경 타임 데이터
	 * @param Target 타겟 환경 타임 데이터
	 * @param Alpha 보간 알파
	 */
	UFUNCTION()
	void LerpFog(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha);

	
	// Initial
	/**
	 * @brief 클래스 값에 따른 초기화()
	 */
	UFUNCTION(CallInEditor, Category="EnvironmentManager")
	void Initialize();

	/**
	 * @brief 포스트 프로세스 초기화
	 */
	UFUNCTION()
	void InitializePostProcess();

	/**
	 * @brief 트랜지션 인덱스 초기화
	 */
	UFUNCTION()
	void InitializeTransitionIndex();

	/**
	 * @brief 시간 데이터 초기화
	 */
	UFUNCTION()
	void InitalizeTimeData();

	/**
	 * @brief 트랜지션 데이터 초기화
	 */
	UFUNCTION()
	void InitializeEnvironmentDataTable();


	// Calculate
	/**
	 * @brief 시간 변수에 따른 썬 라이트 회전값 계산
	 * @param Time 0.0f ~ 24.0f. 24시간 체계 시간
	 * @return 오일러 회전 Y값
	 */
	UFUNCTION()
	float CalculateSunRotation(const float Time);
	
	/**
	 * @brief 시간 변수에 따른 가장 가까운 인덱스 찾기
	 * @param Time 0.0f ~ 24.0f. 24시간 체계 시간
	 * @return 트랜지션 인덱스
	 */
	UFUNCTION()
	int32 FindTimeDataIndexForTime(const float Time);

	/**
	 * @brief 썬 라이트 회전각에 따른 비지빌리티 변경
	 */
	UFUNCTION()
	void UpdateLightVisible();
	
	// Setter
	
	/**
	 * @brief 트랜지션 각 멤버 변수 인덱스 설정에서 사용되는 함수
	 * @param Index 인덱스 참조
	 * @param NewIndex 새로운 인덱스 값
	 */
	UFUNCTION()
	void SetTransitionIndex(int32& Index, int32 NewIndex);

	/**
	 * @brief 현재 시간 설정
	 * @param NewTime 새로운 시간
	 */
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
	/**
	 * @brief 시작 시간
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time", meta=(UIMin = "0.0", UIMax = "24.0"))
	float StartTime = 0.0f;

	/**
	 * @brief 끝 시간
	 */
	// TODO: 끝 시간에 대한 명확한 함수 실행 정의가 이루어지지 않음.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time", meta=(UIMin = "0.0", UIMax = "24.0"))
	float EndTime = 24.0f;
	
	// Component
	/**
	 * @brief 썬 라이트 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Light | Sun")
	UDirectionalLightComponent* Sun;

	/**
	 * @brief 문 라이트 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Light | Moon")
	UDirectionalLightComponent* Moon;

	/**
	 * @brief 볼류메트릭 클라우드 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UVolumetricCloudComponent* Cloud;

	/**
	 * @brief 스카이라이트 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	USkyLightComponent* SkyLight;

	/**
	 * @brief 스카이애트모스피어 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	USkyAtmosphereComponent* SkyAtmosphere;

	/**
	 * @brief 포그 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UExponentialHeightFogComponent* Fog;

	/**
	 * @brief 스카이스피어 메시 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UStaticMeshComponent* SkyMesh;


	// PostProcessVolume
	/**
	 * @brief 포스트 프로세스 볼륨 컴포넌트
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment")
	UPostProcessComponent* PostProcess;

	// EnvironmentTimeData
	/**
	 * @brief 환경 타임 데이터 배열
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentTimeData")
	TArray<FEnvironmentTimeData> EnvironmentTimeDataTable;

public:
	//——————————————————————————————————————————————————————————————————————
	// 함수
	//——————————————————————————————————————————————————————————————————————
	// 오버라이드

	// Debugging
	/**
	 * @brief 디버그용 인덱스
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
	int32 DebuggingIndex = 0;

	/**
	 * @brief 디버그용 인덱스로 트랜지션 실행
	 */
	UFUNCTION(CallInEditor, Category="Debugging")
	void DebugTransitionIndex();

	// Getter
	UFUNCTION(BlueprintCallable, Category="World")
	float GetCurrentTime() { return CurrentTime; }
	
};
