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

const FName AAIControllerCustom::HomePosKey(TEXT("HomePos"));
const FName AAIControllerCustom::PatrolPosKey(TEXT("PatrolPos"));

AAIControllerCustom::AAIControllerCustom()
{
	
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.0f; // 시야 범위
	SightConfig->LoseSightRadius = 1200.0f; // 인지를 멈추는 시야 범위
	SightConfig->PeripheralVisionAngleDegrees = 60.f; // 시야각
	SightConfig->SetMaxAge(5.f); // 정보 유효 기간
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f; // 자동 플레이어 인지 최소 거리

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 적 인지
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 인지
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 우호 인지

	AIPerception->ConfigureSense(*SightConfig); // 퍼셉션 component에 Config추가
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation()); // 청각에 우선순위

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 500.0f; // 청각 범위 설정
	HearingConfig->SetMaxAge(3.f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true; // 적 인지
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 인지
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; // 우호 인지
	AIPerception->ConfigureSense(*HearingConfig);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObj(TEXT("/Game/NPC/BB_AICharacter.BB_AICharacter"));
	if (BBObj.Succeeded())
	{
		BBAsset = BBObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObj(TEXT("/Game/NPC/BT_AICharacter.BT_AICharacter"));
	if (BTObj.Succeeded())
	{
		BTAsset = BTObj.Object;
	}

}

void AAIControllerCustom::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* bbComponent = Blackboard;

	if (UseBlackboard(BBAsset, bbComponent))
	{
		bbComponent->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
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

void AAIControllerCustom::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
	for (AActor* UpdatedActor : UpdatedActors)
	{

		FAIStimulus AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_Sight);

		if (AIStimulus.WasSuccessfullySensed())
		{
			Blackboard->SetValueAsBool(TEXT("bPlayerInSight"), true);
		}
		else
		{
			Blackboard->SetValueAsBool(TEXT("bPlayerInSight"), false);
		}
		
		AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_Hearing);
		if (AIStimulus.WasSuccessfullySensed())
		{
			// 소리 감지 시  그 위치를 SoundLocaiton에 저장
			FVector SoundLocation = AIStimulus.StimulusLocation;
			Blackboard->SetValueAsVector(TEXT("SoundLocation"), SoundLocation);
		}

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
			case EAIPerceptionSense::EPS_Hearing:
				QuerySenseClass = UAISense_Hearing::StaticClass();
				break;
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
