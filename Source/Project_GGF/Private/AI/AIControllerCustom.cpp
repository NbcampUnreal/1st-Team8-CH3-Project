#include "AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/AICharacter.h"
#include "AI/Manager/AIStateManager.h"
#include "AI/Manager/AISenseManager.h"
#include "AI/Manager/AISightHandler.h"


const FName AAIControllerCustom::TargetKey = TEXT("Target");
const FName AAIControllerCustom::PatrolPosKey = TEXT("PatrolPos");
const FName AAIControllerCustom::HomePosKey = TEXT("HomePos");


AAIControllerCustom::AAIControllerCustom()
{
}


void AAIControllerCustom::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("AIController BeginPlay"));

    if (BBAsset && BTAsset)
    {
        if (UseBlackboard(BBAsset, BlackboardComp))
        {
            RunBehaviorTree(BTAsset);
        }
    }

    StateManager = NewObject<UAIStateManager>(this);
    StateManager->Initialize(BlackboardComp, this);

    SenseManager = NewObject<UAISenseManager>(this);
    SenseManager->Initialize(BlackboardComp, this);

    SightHandler = NewObject<UAISightHandler>(this);
    SightHandler->Initialize(this);
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
}


void AAIControllerCustom::OnUnPossess()
{
    Super::OnUnPossess();
    // (나중에 전투 시스템 구현 시 Blackboard 초기화 등 처리)
}


// AI가 순찰할 위치를 설정하는 함수
void AAIControllerCustom::SetPatrolPos(const FVector& NewPatrolPos)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(PatrolPosKey, NewPatrolPos);
    }
}

// AI의 홈 위치를 설정하는 함수
void AAIControllerCustom::SetHomePos(const FVector& NewHomePos)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(HomePosKey, NewHomePos);
    }
}

// AI의 순찰 위치 가져오는 함수
FVector AAIControllerCustom::GetPatrolPos() const
{
    return BlackboardComp ? BlackboardComp->GetValueAsVector(PatrolPosKey) : FVector::ZeroVector;
}

// AI의 홈 위치를 가져오는 함수
FVector AAIControllerCustom::GetHomePos() const
{
    return BlackboardComp ? BlackboardComp->GetValueAsVector(HomePosKey) : FVector::ZeroVector;
}