#include "AI/GGFAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

const FName AGGFAIController::HomePosKey(TEXT("HomePos"));
const FName AGGFAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AGGFAIController::InvestigatePosKey(TEXT("InvestigatePos"));

AGGFAIController::AGGFAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	PerceptionComponent->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	PerceptionComponent->ConfigureSense(*HearingConfig);

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGGFAIController::TargetPerceptionUpdated);
}

void AGGFAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGGFAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGGFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* blackboardComponent = Blackboard;

	if (UseBlackboard(BBAsset, blackboardComponent))
	{
		blackboardComponent->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		RunBehaviorTree(BTAsset);
	}
}

void AGGFAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AGGFAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 시야1, 2할때 쓸 거리
	float Distance = FVector::Dist(GetPawn()->GetActorLocation(), Actor->GetActorLocation());

	
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Target detected: %s"), *Actor->GetName());
		
		Blackboard->SetValueAsVector(InvestigatePosKey, Actor->GetActorLocation());
		Blackboard->SetValueAsBool(TEXT("bSight1"), true);
		
		GetWorld()->GetTimerManager().SetTimer(
			InvestigateUpdateTimer,
			this, 
			&AGGFAIController::UpdateInvestigatePos, 
			0.5f, 
			true 
		);
	}
	else // 대상을 놓쳤을 때 (시야에서 사라졌을 때)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lost target: %s"), *Actor->GetName());

		// 마지막 위치를 InvestigatePos에 유지
		FVector LastSeenPos = Actor->GetActorLocation();
		Blackboard->SetValueAsVector(InvestigatePosKey, LastSeenPos);
		Blackboard->SetValueAsBool(TEXT("bSight1"), false);
		
		GetWorld()->GetTimerManager().ClearTimer(InvestigateUpdateTimer);
	}
}

void AGGFAIController::UpdateInvestigatePos()
{
	TArray<AActor*> SensedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SensedActors);

	if (SensedActors.Num() > 0)
	{
		AActor* TargetActor = SensedActors[0]; // 첫 번째 감지된 액터
		FVector NewInvestigatePos = TargetActor->GetActorLocation();

		Blackboard->SetValueAsVector(InvestigatePosKey, NewInvestigatePos);
		UE_LOG(LogTemp, Warning, TEXT("Updating InvestigatePos: %s"), *NewInvestigatePos.ToString());
	}
}
