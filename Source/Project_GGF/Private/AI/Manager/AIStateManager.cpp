#include "AI/Manager/AIStateManager.h"
#include "AI/AIControllerCustom.h"
#include "Kismet/GameplayStatics.h"


void UAIStateManager::Initialize(UBlackboardComponent* Blackboard, AAIControllerCustom* Controller)
{
	BlackboardComp = Blackboard;
    AIController = Controller;
	CurrentState = ELookState::Idle;
}

void UAIStateManager::ChangeState(ELookState FromState, ELookState ToState)
{
    if (!BlackboardComp) return;

    // 상태 전환 전 유효성 검사
    if (!CanTransitionToState(FromState, ToState))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid state transition from %s to %s"),
            *UEnum::GetValueAsString(FromState), *UEnum::GetValueAsString(ToState));
        return;
    }

    SetBlackboardState(ToState);
    CurrentState = ToState;

    UE_LOG(LogTemp, Warning, TEXT("AI State Transition: %s -> %s"),
        *UEnum::GetValueAsString(FromState), *UEnum::GetValueAsString(ToState));
}

ELookState UAIStateManager::GetCurrentState() const
{
    return CurrentState;
}

bool UAIStateManager::CanTransitionToState(ELookState FromState, ELookState ToState)
{
    // 현재 상태에 따른 다음 전환 가능한 상태 매핑
    static const TMap<ELookState, TArray<ELookState>> ValidTransitions = {
        {ELookState::Idle, {
            ELookState::Patrolling,
            ELookState::Alert,
            ELookState::PlayerTracking
        }},
        {ELookState::Patrolling, {
            ELookState::Alert,
            ELookState::PlayerTracking,
            ELookState::Idle,
            ELookState::SweepingSurroundings
        }},
        {ELookState::Alert, {
            ELookState::PlayerTracking,
            ELookState::Investigating,
            ELookState::Idle,
            ELookState::Searching
        }},
        {ELookState::PlayerTracking, {
            ELookState::Investigating,
            ELookState::Searching,
            ELookState::Alert,
            ELookState::Idle
        }},
        {ELookState::Investigating, {
            ELookState::Searching,
            ELookState::PlayerTracking,
            ELookState::ReturnToHome,
            ELookState::Idle
        }},
        {ELookState::Searching, {
            ELookState::PlayerTracking,
            ELookState::ReturnToHome
        }},
        {ELookState::SweepingSurroundings, {
            ELookState::Patrolling,
            ELookState::Investigating,
            ELookState::Alert
        }},
        {ELookState::ReturnToHome, {
            ELookState::Patrolling,
            ELookState::Idle,
            ELookState::Alert
        }}
    };

    const TArray<ELookState>* ValidNextStates = ValidTransitions.Find(FromState);
    return ValidNextStates && ValidNextStates->Contains(ToState);
}


// AI의 여러 상태가 동시에 활성화되는 문제를 해결
void UAIStateManager::ValidateAIState()
{
    if (!BlackboardComp) return;

    bool bPlayerInSight = BlackboardComp->GetValueAsBool(TEXT("bPlayerInSight"));
    bool bInvestigate = BlackboardComp->GetValueAsBool(TEXT("bInvestigate"));
    bool bPatrolling = BlackboardComp->GetValueAsBool(TEXT("bPatrolling"));
    bool bAlert = BlackboardComp->GetValueAsBool(TEXT("bAlert"));
    bool bSearching = BlackboardComp->GetValueAsBool(TEXT("bSearching"));
    bool bReturnToHome = BlackboardComp->GetValueAsBool(TEXT("bReturnToHome"));

    int ActiveStateCount = (bPlayerInSight ? 1 : 0) +
        (bInvestigate ? 1 : 0) +
        (bPatrolling ? 1 : 0) +
        (bAlert ? 1 : 0) +
        (bSearching ? 1 : 0) +
        (bReturnToHome ? 1 : 0);

    if (ActiveStateCount > 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Resetting to valid state"));
        SetBlackboardState(CurrentState);
    }
}


void UAIStateManager::ResetBlackboardStates()
{
    if (!BlackboardComp) return;

    BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), false);
    BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), false);
    BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), false);
    BlackboardComp->SetValueAsBool(TEXT("bAlert"), false);
    BlackboardComp->SetValueAsBool(TEXT("bSearching"), false);
    BlackboardComp->SetValueAsBool(TEXT("bReturnToHome"), false);
    BlackboardComp->SetValueAsBool(TEXT("bSweepingSurroundings"), false);
}


void UAIStateManager::SetBlackboardState(ELookState NewState)
{
    if (!BlackboardComp) return;

    ResetBlackboardStates();

    switch (NewState)
    {
    case ELookState::Patrolling:
        BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), true);
        break;
    case ELookState::Alert:
        BlackboardComp->SetValueAsBool(TEXT("bAlert"), true);
        break;
    case ELookState::PlayerTracking:
        BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), true);
        break;
    case ELookState::Investigating:
        BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), true);
        break;
    case ELookState::Searching:
        BlackboardComp->SetValueAsBool(TEXT("bSearching"), true);
        break;
    case ELookState::ReturnToHome:
        BlackboardComp->SetValueAsBool(TEXT("bReturnToHome"), true);
        break;
    case ELookState::SweepingSurroundings:
        BlackboardComp->SetValueAsBool(TEXT("bSweepingSurroundings"), true);
        break;
    case ELookState::Idle:
        break;
    }

    UE_LOG(LogTemp, Warning, TEXT("Blackboard Updated for State: %s"), *UEnum::GetValueAsString(NewState));
}

/*
* ai의 현재 시야 상태 업데이트
* SenseDataMap에서 ESenseType::Sight 데이터를 찾아 현재 상태를 업데이트
* AI의 상태 변화를 UE_LOG로 출력하여 디버깅
*/
void UAIStateManager::UpdateLookState(ELookState NewState)
{
    if (CurrentState == NewState) return;

    CurrentState = NewState;
    SetBlackboardState(NewState);

    UE_LOG(LogTemp, Warning, TEXT("AI Look State Updated: %s"), *UEnum::GetValueAsString(NewState));
}

// AI의 상태를 강제로 조사 상태로 변경
void UAIStateManager::ForceInvestigate()
{
    if (!BlackboardComp) return;

    ResetBlackboardStates();
    BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), true); // 조사 상태로 강제로 변경
    UpdateLookState(ELookState::Investigating);

    // 마지막 위치로 이동
    FVector LastKnownPos = BlackboardComp->GetValueAsVector(TEXT("LastKnownPos"));
    if (!LastKnownPos.IsZero())
    {
        UE_LOG(LogTemp, Warning, TEXT("ForceInvestigate: 마지막 위치(%s)로 강제 이동"), *LastKnownPos.ToString());
        AIController->MoveToLocation(LastKnownPos, -1.0f, true);
    }
}

void UAIStateManager::HandlePlayerLost()
{
    if (!BlackboardComp) return;

    if (BlackboardComp->GetValueAsBool(TEXT("bPlayerInSight")))
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIController->GetWorld(), 0);
        if (PlayerPawn)
        {
            UpdateLookState(ELookState::Investigating);
            BlackboardComp->SetValueAsVector(TEXT("LastKnownPos"), PlayerPawn->GetActorLocation());

            BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), false);
            BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), true);

            AIController->MoveToLocation(PlayerPawn->GetActorLocation());
        }
    }
}