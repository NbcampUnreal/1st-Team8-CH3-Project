// Fill out your copyright notice in the Description page of Project Settings.


#include "World/EnvironmentManager.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "MaterialHLSLTree.h"


// Sets default values
AEnvironmentManager::AEnvironmentManager()
{
	//Initialize
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootComponent
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Sun
	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	Sun->SetupAttachment(RootComponent);

	// Moon
	Moon = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Moon"));
	Moon->SetupAttachment(Sun);

	// Cloud
	Cloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("Cloud"));
	Cloud->SetupAttachment(RootComponent);

	//SkyLight
	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyLight->SetupAttachment(RootComponent);

	// Fog
	Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Fog"));
	Fog->SetupAttachment(RootComponent);

	// 포스트 프로세스
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(RootComponent);

	// 포스트 프로세스 설정 활성화
	PostProcess->Settings.bOverride_WhiteTemp = true;
	
	PostProcess->Settings.bOverride_ColorSaturation = true;
	PostProcess->Settings.bOverride_ColorContrast = true;
	PostProcess->Settings.bOverride_ColorGamma = true;
	PostProcess->Settings.bOverride_ColorGain = true;
	PostProcess->Settings.bOverride_ColorOffset = true;

	PostProcess->Settings.bOverride_AutoExposureMinBrightness = true;
	PostProcess->Settings.bOverride_AutoExposureMaxBrightness = true;

	// Sky Attmosphere
	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>("SkyAtmosphere");
	SkyAtmosphere->SetupAttachment(RootComponent);

	// Sky Mesh
	SkyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkyMesh"));

	
	// Initialize
	//Time
	//시간 검사
	SetCurrentTime(CurrentTime);
	
	// Sun
	Sun->Intensity = 4.0f;
	Sun->AtmosphereSunLightIndex = 0;
	Moon->ForwardShadingPriority = 0;
	
	// Moon
	// 해 라이트와 반대급부 위치
	Moon->SetWorldRotation(FRotator(180, 0, 0));
	Moon->SetVisibility(true);
	
	Moon->Intensity = 0.25f;
	Moon->LightSourceAngle = 0.0f;
	Moon->AtmosphereSunLightIndex = 1;
	Moon->ForwardShadingPriority = 1;
	
	Moon->bUseTemperature = true;
	Moon->Temperature = 12000.0f;
	
	// Cloud

	
	// Sky Light
	// 스카이라이트 내 볼류메트릭 클라우드와 스카이 애트머스피어의 색상 캡쳐
	SkyLight->SetRealTimeCapture(true);

	// Fog
	// 볼류메트릭 설정
	Fog->SetVolumetricFog(true);
	Fog->FogDensity = 0.03;

	// Sky Atmosphere

	// 초기화
	Initialize();
}


// Overide
void AEnvironmentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 
	TimeUpdate(DeltaTime);
	TimeCheck();
	TransitionUpdate(DeltaTime);
}

void AEnvironmentManager::BeginPlay()
{
	Super::BeginPlay();

	// 이니셜라이즈 함수 호출
	Initialize();
	
}


// Time
void AEnvironmentManager::TimeUpdate(const float DeltaTime)
{
	// 세계 시간 규칙
	RealElapsedTime += DeltaTime;
	CurrentTime += DeltaTime / SecondsPerGameHour;

	// 하루가 끝나면 처음으로 돌아가기
	SetCurrentTime(CurrentTime);
}

void AEnvironmentManager::TimeCheck()
{
	if (RealElapsedTime > 0.5f)
	{
		if (not bIsTransitioning and bExistTransitionData)
		{
			float CheckingTime = TimeDivision * NextIndex;
	
			if (FMath::IsNearlyEqual(CheckingTime, CurrentTime, 0.1f))
			{
				StartTransition();
			}
		}
	}
}



// Transition
void AEnvironmentManager::StartTransition()
{
	// 진행 제어 변수
	TransitionProgress = 0.0f;
	bIsTransitioning = true;
}

void AEnvironmentManager::TransitionUpdate(const float DeltaTime)
{
	if (bIsTransitioning)
	{
		// 보간 알파 생성
		TransitionProgress += DeltaTime / TransitionDuration;

		FEnvironmentTimeData& CurrentData = EnvironmentTimeDataTable[CurrentIndex];
		FEnvironmentTimeData& NextData = EnvironmentTimeDataTable[NextIndex];
		
		// Env 데이터
		if (TransitionProgress >= 1.0f)
		{
			// 마지막 1.0f로 고정
			// 설정 보간
			LerpLightRotation(CurrentData,NextData,1);
			LerpPostProcess(CurrentData,NextData,1);
			LerpFog(CurrentData,NextData,1);
			
			// 트랜지션 진행 초기화
			StopTransition();
		}
		
		// 설정 보간
		LerpLightRotation(CurrentData,NextData,TransitionProgress);
		LerpPostProcess(CurrentData,NextData,TransitionProgress);
		LerpFog(CurrentData,NextData,TransitionProgress);

		UpdateLightVisible();
	}
}

void AEnvironmentManager::StopTransition()
{
	TransitionProgress = 1.0f;

	// 인덱스 증가
	SetTransitionIndex(PreviousIndex, CurrentIndex);
	SetTransitionIndex(CurrentIndex, NextIndex);
	SetTransitionIndex(NextIndex, CurrentIndex + 1);

	// 인덱스 순회 지정
	if (NextIndex > TotalIndex) NextIndex = 0;
	
	bIsTransitioning = false;
}


// Debugging Runtime Transition
void AEnvironmentManager::NextTransition()
{
	float NextTime = EnvironmentTimeDataTable[NextIndex].Time;
	SetCurrentTime(NextTime);
	
	StartTransition();
}

void AEnvironmentManager::PreviousTransition()
{
	// TODO: 이전 인데스 트랜지션 실행
}

void AEnvironmentManager::ViewTransition(int32 Index)
{
	// 인덱스 검사
	if (Index > TotalIndex) Index = TotalIndex;
	if (Index < 0) Index = 0;
	
	// 배열 데이터로 적용
	const FEnvironmentTimeData& Data = EnvironmentTimeDataTable[Index];

	// StartTime으로 설정
	SetCurrentTime(Data.Time);
	
	// 해 로테이션 초기화
	float NewPitch = CalculateSunRotation(Data.Time);
	float NewRotationCoefficient = Data.TimeRotationCoefficient;
	FRotator NewRotation = FRotator::MakeFromEuler(FVector(0, NewPitch + NewRotationCoefficient, 0));
	Sun->SetWorldRotation(NewRotation);
	
	// 포스트 프로세스 구조체 초기화
	PostProcess->Settings = Data.PostProcess.ToPostProcessSettings();

	// 포그 초기화
	Fog->SetFogDensity(Data.FogDensity);
	Fog->SetFogHeightFalloff(Data.FogHeightFalloff);
}


// Lerp
void AEnvironmentManager::LerpLightRotation(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha)
{
	// Sun 설정 적용
	if (Sun)
	{
		float CurrentY = CalculateSunRotation(Current.Time);
		float TargetY = CalculateSunRotation(Target.Time);

		float CurrentRotationAdd = Current.TimeRotationCoefficient;
		float TargetRotationAdd = Target.TimeRotationCoefficient;
		FRotator CurrentRotation = FRotator::MakeFromEuler(FVector(0, CurrentY + CurrentRotationAdd, 0));
		FRotator TargetRotation = FRotator::MakeFromEuler(FVector(0, TargetY + TargetRotationAdd, 0));

		FRotator NewRotation = FRotator(FQuat::Slerp(CurrentRotation.Quaternion(),TargetRotation.Quaternion(),Alpha));
		Sun->SetWorldRotation(NewRotation);
	}
}

void AEnvironmentManager::LerpPostProcess(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha)
{
	// PostProcess 설정 적용
	if (PostProcess)
	{
		// 구조체 선형 보간
		FEnvironmentPostProcess NewPostProcess = Current.PostProcess.LerpWith(Target.PostProcess, Alpha);

		// 대입
		PostProcess->Settings = NewPostProcess.ToPostProcessSettings();
	}
}

void AEnvironmentManager::LerpFog(const FEnvironmentTimeData& Current, const FEnvironmentTimeData& Target, const float& Alpha)
{
	// Fog 설정 적용
	 if (Fog)
	 {
	 	Fog->SetFogDensity(FMath::Lerp(Current.FogDensity, Target.FogDensity, Alpha));
		Fog->SetFogHeightFalloff(FMath::Lerp(Current.FogHeightFalloff, Target.FogHeightFalloff, Alpha));
	 }
}


// Initial
void AEnvironmentManager::Initialize()
{
	// 환경 데이터 정렬 -> 트랜지션 인덱싱 -> 타임 데이터 셋
	InitializePostProcess();
	InitializeEnvironmentDataTable();
	InitializeTransitionIndex();
	InitalizeTimeData();
}

void AEnvironmentManager::InitializePostProcess()
{
	// 포스트 프로세스 세팅 부울 값 활성화
	PostProcess->Settings.bOverride_WhiteTemp = true;
	
	PostProcess->Settings.bOverride_ColorSaturation = true;
	PostProcess->Settings.bOverride_ColorContrast = true;
	PostProcess->Settings.bOverride_ColorGamma = true;
	PostProcess->Settings.bOverride_ColorGain = true;
	PostProcess->Settings.bOverride_ColorOffset = true;

	PostProcess->Settings.bOverride_AutoExposureMinBrightness = true;
	PostProcess->Settings.bOverride_AutoExposureMaxBrightness = true;

}

void AEnvironmentManager::InitializeEnvironmentDataTable()
{
	// 배열 정렬 실행
	EnvironmentTimeDataTable.Sort
	(
		[](const FEnvironmentTimeData& A, const FEnvironmentTimeData& B)
		{
			return A.Priority < B.Priority;  // 우선순위 오름차순 정렬
		}
	);

	// 배열 내 포스트 프로세스 부울 활성화
	for (FEnvironmentTimeData& Data : EnvironmentTimeDataTable)
	{
		Data.PostProcess.bOverride_WhiteTemp = true;
		Data.PostProcess.bOverride_ColorSaturation = true;
		Data.PostProcess.bOverride_ColorContrast = true;
		Data.PostProcess.bOverride_ColorGamma = true;
		Data.PostProcess.bOverride_ColorGain = true;
		Data.PostProcess.bOverride_ColorOffset = true;
		Data.PostProcess.bOverride_AutoExposureMinBrightness = true;
		Data.PostProcess.bOverride_AutoExposureMaxBrightness = true;
	}
}

void AEnvironmentManager::InitializeTransitionIndex()
{
	const int32 SliceCount = EnvironmentTimeDataTable.Num();
	if (SliceCount <= 0)
	{
		bExistTransitionData = false;
		return;
	}
    
	// 24시간을 슬라이스 수로 나누어 균등 배분
	bExistTransitionData = true;
	TotalIndex = SliceCount;
	TimeDivision = 24.0f / SliceCount;

	for (FEnvironmentTimeData& Data : EnvironmentTimeDataTable)
	{
		Data.Time = TimeDivision * Data.Priority;
	}
}

void AEnvironmentManager::InitalizeTimeData()
{
	if (bExistTransitionData)
	{
		bIsTransitioning = false;
		
		// StartTime으로 설정
		SetCurrentTime(StartTime);

		int32 NewIndex = FindTimeDataIndexForTime(CurrentTime);

		// StartTime에 해당하는 적절한 인덱스 찾기
		SetTransitionIndex(PreviousIndex, CurrentIndex);
		SetTransitionIndex(CurrentIndex, NewIndex);
		SetTransitionIndex(NextIndex, CurrentIndex + 1);
	
		// 트랜지션 인덱스 실행
		ViewTransition(CurrentIndex);

		UpdateLightVisible();
	}
}


// Calculate
float AEnvironmentManager::CalculateSunRotation(const float Time)
{
	// 24시간 기준
	float Result = (360.0f * (Time * (1/24.0f)) + 90.0f);
	return Result;
}

int32 AEnvironmentManager::FindTimeDataIndexForTime(const float Time)
{
	if (!bExistTransitionData || EnvironmentTimeDataTable.Num() == 0) return -1;
    
	// 균등 분할된 시간 기준으로 인덱스 계산
	int32 Index = FMath::FloorToInt(Time / TimeDivision);
	
	// 인덱스 범위 제한
	return FMath::Clamp(Index, 0, TotalIndex);
}

void AEnvironmentManager::UpdateLightVisible()
{
	float SunRotationY = Sun->GetComponentRotation().Euler().Y;
	
	// 선 셋
	if (SunRotationY >= 7.0f)
	{
		Sun->SetVisibility(false);
		Moon->SetVisibility(true);
	}
	
	if (SunRotationY <= -7.0f)
	{
		Sun->SetVisibility(true);
		Moon->SetVisibility(false);
	}
	if (SunRotationY < 7.0f and SunRotationY > -7.0f)
	{
		Sun->SetVisibility(true);
		Moon->SetVisibility(true);
	}
}


// Setter
void AEnvironmentManager::SetTransitionIndex(int32& Index, int32 NewIndex)
{
	Index = NewIndex;
	
	if (Index >= TotalIndex) Index = 0;
	if (Index < 0) Index = TotalIndex;
}

void AEnvironmentManager::SetCurrentTime(float NewTime)
{
	// 24시간 시스템에서 순환 처리
	CurrentTime = FMath::Fmod(NewTime, 24.0f);
    
	// 음수 시간 처리 (만약 NewTime이 음수일 경우)
	if (CurrentTime < 0.0f)
	{
		CurrentTime += 24.0f;
	}
}


// Debugging
void AEnvironmentManager::DebugTransitionIndex()
{
	if (bExistTransitionData)
	{
		bIsTransitioning = false;

		// StartTime에 해당하는 적절한 인덱스 찾기
		SetTransitionIndex(PreviousIndex, DebuggingIndex - 1);
		SetTransitionIndex(CurrentIndex, DebuggingIndex);
		SetTransitionIndex(NextIndex, DebuggingIndex + 1);

		// 트랜지션 인덱스 실행
		ViewTransition(CurrentIndex);

		// 비지빌리티 최적화
		UpdateLightVisible();
	}
}

