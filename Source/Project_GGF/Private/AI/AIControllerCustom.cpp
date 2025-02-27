#include "AIControllerCustom.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/AICharacter.h"
#include "AI/Manager/AIStateManager.h"
#include "AI/Manager/AISenseManager.h"
#include "AI/Manager/AISightHandler.h"
#include "AI/Manager/AIHearingHandler.h"

const FName AAIControllerCustom::TargetKey = TEXT("Target");
const FName AAIControllerCustom::PatrolPosKey = TEXT("PatrolPos");
const FName AAIControllerCustom::HomePosKey = TEXT("HomePos");


AAIControllerCustom::AAIControllerCustom()
{
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}


void AAIControllerCustom::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("AIController BeginPlay"));

}


void AAIControllerCustom::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
#if WITH_EDITOR
    if (SenseManager)
    {
        SenseManager->DrawDebugSenseInfo();
    }
#endif
}


void AAIControllerCustom::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BBAsset && BTAsset)
    {
        if (UseBlackboard(BBAsset, BlackboardComp))
        {
            BlackboardComp->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
            BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), true);
            RunBehaviorTree(BTAsset);
        }
    }

    if (!StateManager)
    {
        StateManager = NewObject<UAIStateManager>(this);
        StateManager->Initialize(BlackboardComp, this);
    }

    if (!SenseManager)
    {
        SenseManager = NewObject<UAISenseManager>(this);
        SenseManager->Initialize(BlackboardComp, this);
    }

    if (!SightHandler)
    {
        SightHandler = NewObject<UAISightHandler>(this);
        SightHandler->Initialize(this, AIPerception);
    }

    if (!HearingHandler)
    {
        HearingHandler = NewObject<UAIHearingHandler>(this);
        HearingHandler->Initialize(this, AIPerception);
    }
}


void AAIControllerCustom::OnUnPossess()
{
    Super::OnUnPossess();
    // (나중에 전투 시스템 구현 시 Blackboard 초기화 등 처리)
}

// AI의 홈 위치를 설정하는 함수
void AAIControllerCustom::SetHomePos(const FVector& NewHomePos)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(HomePosKey, NewHomePos);
    }
}

// AI의 홈 위치를 가져오는 함수
FVector AAIControllerCustom::GetHomePos() const
{
    return BlackboardComp ? BlackboardComp->GetValueAsVector(HomePosKey) : FVector::ZeroVector;
}