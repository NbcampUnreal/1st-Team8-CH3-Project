#include "AI/HearingControl.h"
#include "AI/AIControllerCustom.h"
#include "AI/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/GameplayStatics.h"

UHearingControl::UHearingControl()
{
	PrimaryComponentTick.bCanEverTick = false;

	OwnerController = nullptr;
	OwnerBlackboard = nullptr;
	OwnerPerception = nullptr;

}

void UHearingControl::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AAIControllerCustom>(GetOwner());

	if (!OwnerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("HearingControl : AIController is not found"))
			return;
	}

	OwnerBlackboard = OwnerController->GetBlackboardComponent();
	OwnerPerception = OwnerController->AIPerception;

	if (!OwnerPerception)
	{
		UE_LOG(LogTemp, Warning, TEXT("HearingControl : AI Perception Component is not found"))
			return;
	}

	OwnerPerception->OnTargetPerceptionUpdated.AddDynamic(this, &UHearingControl::OnPerceptionUpdated);

}

void UHearingControl::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	if (Stimulus.Type != UAISense::GetSenseID<UAISense_Hearing>()) return;

	// 같은 종족인지 확인
	ENoiseSourceType SourceType = ClassifyNoiseSource(Actor);
	if (SourceType == ENoiseSourceType::SameSpecies) return;

	float NoiseStrength = CalculateNoiseStrength(Stimulus);

	if (NoiseStrength < MinNoiseThreshold) return;

	UE_LOG(LogTemp, Warning, TEXT("소리 감지! 강도: %.1f, 위치: %s, 종류: %s"),
		NoiseStrength, *Stimulus.StimulusLocation.ToString(),
		*UEnum::GetValueAsString(SourceType));

	if (OwnerBlackboard)
	{
		OwnerBlackboard->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
		OwnerBlackboard->SetValueAsBool("bHeardNoise", true);
	}

	// 감각 데이터 업데이트
	OwnerController->UpdateSenseData(ESenseType::Hearing, true, Stimulus.StimulusLocation);

	DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 50.0f, 12, FColor::Green, false, 5.0f);
	FVector AIPosition = OwnerController->GetPawn()->GetActorLocation();
	DrawDebugLine(GetWorld(), AIPosition, Stimulus.StimulusLocation, FColor::Blue, false, 5.0f, 0, 2.0f);
}

ENoiseSourceType UHearingControl::ClassifyNoiseSource(AActor* Source) const
{

	AAICharacter* OwnerCharacter = Cast<AAICharacter>(OwnerController->GetPawn());

	if (!OwnerCharacter) return ENoiseSourceType::Other;

	// 소스가 AI 캐릭터인지 확인
	AAICharacter* SourceCharacter = Cast<AAICharacter>(Source);

	if (SourceCharacter && SourceCharacter->AIType == OwnerCharacter->AIType)
	{
		return ENoiseSourceType::SameSpecies;
	}

	return ENoiseSourceType::Other;
}

float UHearingControl::CalculateNoiseStrength(const FAIStimulus& Stimulus) const
{

	APawn* OwnerPawn = OwnerController ? OwnerController->GetPawn() : nullptr;

	if (!OwnerPawn) return Stimulus.Strength;

	float Distance = FVector::Dist(OwnerPawn->GetActorLocation(), Stimulus.StimulusLocation);
	float MaxRange = OwnerController->HearingConfig->HearingRange;
	float NormalizedDistance = FMath::Clamp(Distance / MaxRange, 0.0f, 1.0f);

	// 거리에 따른 강도를 1 ~ 10으로 조정
	float NoiseStrength = 10.0f * (1.0f - NormalizedDistance) * Stimulus.Strength;

	return FMath::Clamp(NoiseStrength, 1.0f, 10.0f);
}