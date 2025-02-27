#include "AI/Manager/AIHearingHandler.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Manager/AISenseManager.h"
#include "AI/AICharacter.h"

UAIHearingHandler::UAIHearingHandler()
{
	PrimaryComponentTick.bCanEverTick = false;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	HearingConfig->HearingRange = HearingRadius;
	HearingConfig->SetMaxAge(5.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*HearingConfig);
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &UAIHearingHandler::HearingPerceptionUpdated);
}

void UAIHearingHandler::Initialize(AAIControllerCustom* InController)
{
	AIController = InController;
	SenseManager = AIController->GetSenseManager();
	BlackboardComp = AIController->GetBlackboardComponent();
}

void UAIHearingHandler::HearingPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type != UAISense::GetSenseID<UAISense_Hearing>()) return;

	// 같은 종족의 소음이면 무시
	if (IsSameSpecies(Actor)) return;

	float NoiseStrength = CalculateNoiseStrength(Stimulus);
	if (NoiseStrength < MinNoiseThreshold) return;

	UE_LOG(LogTemp, Warning, TEXT("소리 감지! 강도: %.1f, 위치: %s"), NoiseStrength, *Stimulus.StimulusLocation.ToString());

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
		BlackboardComp->SetValueAsBool("bHeardNoise", true);
	}

	if (SenseManager)
	{
		SenseManager->UpdateSenseData(ESenseType::Hearing, true, Stimulus.StimulusLocation);
	}

	DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 50.0f, 12, FColor::Green, false, 5.0f);
	FVector AIPosition = AIController->GetPawn()->GetActorLocation();
	DrawDebugLine(GetWorld(), AIPosition, Stimulus.StimulusLocation, FColor::Blue, false, 5.0f, 0, 2.0f);
}


bool UAIHearingHandler::IsSameSpecies(AActor* Source) const
{
	if (!AIController) return false;

	AAICharacter* OwnerCharacter = Cast<AAICharacter>(AIController->GetPawn());
	if (!OwnerCharacter) return false;

	AAICharacter* SourceCharacter = Cast<AAICharacter>(Source);
	return (SourceCharacter && SourceCharacter->AIType == OwnerCharacter->AIType);
}

float UAIHearingHandler::CalculateNoiseStrength(const FAIStimulus& Stimulus) const
{
	if (!AIController) return Stimulus.Strength;

	APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn) return Stimulus.Strength;

	float Distance = FVector::Dist(OwnerPawn->GetActorLocation(), Stimulus.StimulusLocation);
	float MaxRange = HearingRadius;
	float NormalizedDistance = FMath::Clamp(Distance / MaxRange, 0.0f, 1.0f);

	// 거리에 따른 강도를 1 ~ 10으로 조정
	float NoiseStrength = 10.0f * (1.0f - NormalizedDistance) * Stimulus.Strength;

	return FMath::Clamp(NoiseStrength, 1.0f, 10.0f);
}