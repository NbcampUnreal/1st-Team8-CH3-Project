#include "AIControllerCustom.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

const FName AAIControllerCustom::HomePosKey(TEXT("HomePos"));
const FName AAIControllerCustom::PatrolPosKey(TEXT("PatrolPos"));
const FName AAIControllerCustom::TargetKey(TEXT("Target"));

AAIControllerCustom::AAIControllerCustom()
{

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 400.0f; // 시야 범위
	SightConfig->LoseSightRadius = 420.0f; // 인지를 멈추는 시야 범위
	SightConfig->PeripheralVisionAngleDegrees = 60.f; // 시야각
	SightConfig->SetMaxAge(0.5f); // 정보 유효 기간
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f; // 자동 플레이어 인지 최소 거리

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 적 인지
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 인지
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 우호 인지

	AIPerception->ConfigureSense(*SightConfig); // 퍼셉션 component에 Config추가
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation()); // 시각에 우선순위
	/*
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 500.0f; // 청각 범위 설정
	HearingConfig->SetMaxAge(3.f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true; // 적 인지
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 인지
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; // 우호 인지
	AIPerception->ConfigureSense(*HearingConfig);
	*/

}

void AAIControllerCustom::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BBAsset)
	{
		UBlackboardComponent* BlackboardPtr = Blackboard.Get();
		if (UseBlackboard(BBAsset, BlackboardPtr))
		{
			Blackboard = BlackboardPtr; 
			Blackboard->SetValueAsVector("HomePos", InPawn->GetActorLocation());

			if (BTAsset)
			{
				if (!RunBehaviorTree(BTAsset))
				{
					UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
				}
			}
		}
	}

}

void AAIControllerCustom::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAIControllerCustom::BeginPlay()
{
	Super::BeginPlay();
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAIControllerCustom::PerceptionUpdated);
}

void AAIControllerCustom::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	FVector StartLocation = ControlledPawn->GetActorLocation() + FVector(0, 0, 50);
	FVector ForwardVector = ControlledPawn->GetActorForwardVector();

	float FOVAngle = SightConfig->PeripheralVisionAngleDegrees * 0.5f;
	float VisionRange = SightConfig->SightRadius;
	// ai 시야각 원뿔 형태
	DrawDebugCone(
		GetWorld(),
		StartLocation,
		ForwardVector,
		VisionRange,
		FMath::DegreesToRadians(FOVAngle), // 좌측 반각
		FMath::DegreesToRadians(FOVAngle), // 우측 반각
		32,
		FColor::Red,
		false,
		0.1f
	);

}


void AAIControllerCustom::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

	if (!Blackboard) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn) return;

	// 기본적으로 false로 설정
	bool bDetected = false;

	for (AActor* UpdatedActor : UpdatedActors)
	{
		if (UpdatedActor != PlayerPawn) continue;

		FAIStimulus AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_Sight);

		if (AIStimulus.WasSuccessfullySensed())
		{
			bDetected = true;
			Blackboard->SetValueAsObject(TEXT("Target"), PlayerPawn);
			Blackboard->SetValueAsBool(TEXT("bPlayerInSight"), true);
			GetWorld()->GetTimerManager().ClearTimer(LostSightTimerHandle);
			break;
		}

	}

	if (!bDetected)
	{
		HandleLostSight();
	}
}

FAIStimulus AAIControllerCustom::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense) // 감지할 Actor 어떤 감각으로 감지할지 선택
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo; //Actor 정보 인식
	FAIStimulus ResultStimulus;

	AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo); // Actor에 대한 감지 상태를 가져옴

	TSubclassOf<UAISense> QuerySenseClass; // 감지하려는 특정 감각을 나타냄
	switch (AIPerceptionSense)
	{
	case EAIPerceptionSense::EPS_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	//case EAIPerceptionSense::EPS_Hearing:
	//	QuerySenseClass = UAISense_Hearing::StaticClass();
		//break;
	default:
		break;
	}

	TSubclassOf<UAISense> LastSensedStimulusClass; // 이전 감각 종류
	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus); // 어떤 감각이 사용되었는지 확인

		if (QuerySenseClass == LastSensedStimulusClass) // 요청한 감각과 감지된 감각 일치 여부
		{
			ResultStimulus = AIStimulus;
			return ResultStimulus;
		}
	}
	return ResultStimulus;
}

void AAIControllerCustom::HandleLostSight()
{
	if (!Blackboard) return;

	Blackboard->SetValueAsBool(TEXT("bPlayerInSight"), false);
	Blackboard->ClearValue(TEXT("Target"));

}