#include "AIControllerCustom.h"
#include "AI/HearingControl.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "AI/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

const FName AAIControllerCustom::TargetKey = TEXT("Target");
const FName AAIControllerCustom::PatrolPosKey = TEXT("PatrolPos");
const FName AAIControllerCustom::HomePosKey = TEXT("HomePos");

AAIControllerCustom::AAIControllerCustom()
{
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    SightConfig->SightRadius = DetectionRadius;
    SightConfig->LoseSightRadius = DetectionRadius + 100.0f;
    SightConfig->PeripheralVisionAngleDegrees = MaxSightAngle;
    SightConfig->SetMaxAge(0.1f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 100.0f;

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = HearingRadius;
    HearingConfig->SetMaxAge(5.0f);

    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerception->ConfigureSense(*HearingConfig);
    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

    AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAIControllerCustom::PerceptionUpdated);

    HearingControlComp = CreateDefaultSubobject<UHearingControl>(TEXT("HearingControlComp"));

    // 감각 데이터 초기화
    InitializeSenseData();
}

void AAIControllerCustom::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("OnPossess: InPawn is NULL!"));
        return;
    }

    ControlledAICharacter = Cast<AAICharacter>(InPawn);
    if (!ControlledAICharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("OnPossess: Failed to cast to AICharacter"));
        return;
    }

    if (BBAsset && BTAsset)
    {
        UBlackboardComponent* BlackboardPtr = GetBlackboardComponent();
        if (UseBlackboard(BBAsset, BlackboardPtr))
        {
            BlackboardPtr->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
            BlackboardPtr->SetValueAsBool(TEXT("bPatrolling"), true);
            RunBehaviorTree(BTAsset);
        }
    }
}

void AAIControllerCustom::OnUnPossess()
{
    Super::OnUnPossess();
    // (나중에 전투 시스템 구현 시 Blackboard 초기화 등 처리)
}

void AAIControllerCustom::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("AIController BeginPlay"));

    // 시작할 때 초기상태를 설정
    UpdateLookState(ELookState::Patrolling);
}

bool AAIControllerCustom::CanTransitionToState(ELookState CurrentStateParam, ELookState NewState)
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

    const TArray<ELookState>* ValidNextStates = ValidTransitions.Find(CurrentStateParam);
    if (!ValidNextStates)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid current state for transition: %s"),
            *UEnum::GetValueAsString(CurrentStateParam));
        return false;
    }

    bool bCanTransition = ValidNextStates->Contains(NewState);

    if (!bCanTransition)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Invalid state transition from %s to %s"),
            *UEnum::GetValueAsString(CurrentStateParam),
            *UEnum::GetValueAsString(NewState)
        );
    }

    return bCanTransition;
}

/*
*  ai 상태 변경
*/

void AAIControllerCustom::HandleStateTransition(ELookState FromState, ELookState ToState)
{
    // 상태 전환 전 유효성 검사
    if (!CanTransitionToState(FromState, ToState))
    {
        UE_LOG(LogTemp, Error,
            TEXT("Attempted invalid state transition from %s to %s"),
            *UEnum::GetValueAsString(FromState),
            *UEnum::GetValueAsString(ToState)
        );
        return;
    }

    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
    if (!BlackboardComp) return;

    // 모든 상태 관련 불 값 초기화
    BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), false);
    BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), false);
    BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), false);
    BlackboardComp->SetValueAsBool(TEXT("bAlert"), false);
    BlackboardComp->SetValueAsBool(TEXT("bSearching"), false);
    BlackboardComp->SetValueAsBool(TEXT("bReturnToHome"), false);
    BlackboardComp->SetValueAsBool(TEXT("bSweepingSurroundings"), false);

    // 새로운 상태에 따른 블랙보드 값 설정
    switch (ToState)
    {
    case ELookState::Patrolling:
        BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), true);
        break;
    case ELookState::Alert:
        BlackboardComp->SetValueAsBool(TEXT("bAlert"), true);
        BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), true);
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

    // 현재 상태 업데이트
    CurrentLookState = ToState;

    // 상태 전환 로그
    UE_LOG(LogTemp, Warning, TEXT("AI State Transition: %s -> %s"),
        *UEnum::GetValueAsString(FromState),
        *UEnum::GetValueAsString(ToState)
    );
}

/*
*  ai가 여러 상태를 동시에 가지지 않는지 확인하는 함수
*/
void AAIControllerCustom::ValidateAIState()
{
    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
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

    // 여러 개의 상태가 활성화될 시 초기화 후 복구
    if (ActiveStateCount > 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI State Conflict Detected - Resetting to valid state"));

        BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), false);
        BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), false);
        BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), false);
        BlackboardComp->SetValueAsBool(TEXT("bAlert"), false);
        BlackboardComp->SetValueAsBool(TEXT("bSearching"), false);
        BlackboardComp->SetValueAsBool(TEXT("bReturnToHome"), false);

        switch (CurrentLookState)
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
        }
    }
}

/*
*  AI가 목표 방향으로 부드럽게 회전하도록 보간하는 함수
*/ 
FRotator AAIControllerCustom::SmoothRotateTowards(const FRotator& CurrentRotation, const FRotator& TargetRotation, float DeltaSeconds)
{
    float RotationSpeed = 2.0f;
    float InterpolationFactor = FMath::Clamp(DeltaSeconds * RotationSpeed, 0.0f, 1.0f);
    float InterpYaw = FMath::Lerp(CurrentRotation.Yaw, TargetRotation.Yaw, FMath::SmoothStep(0.0f, 1.0f, InterpolationFactor));
    float InterpPitch = FMath::Lerp(CurrentRotation.Pitch, TargetRotation.Pitch, FMath::SmoothStep(0.0f, 1.0f, InterpolationFactor));
    float InterpRoll = FMath::Lerp(CurrentRotation.Roll, TargetRotation.Roll, FMath::SmoothStep(0.0f, 1.0f, InterpolationFactor));
    return FRotator(InterpPitch, InterpYaw, InterpRoll);
}

void AAIControllerCustom::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
#if WITH_EDITOR
    DrawDebugSenseInfo();
#endif
}

/*
*  플레이어가 ai시야에서 사라졌을 때, 일정 시간 후에 조사 상태 변경
*/

void AAIControllerCustom::OnPlayerInSightTimerFinished()
{
    UBlackboardComponent* BlackboardPtr = GetBlackboardComponent();
    if (!BlackboardPtr) return;

    if (BlackboardPtr->GetValueAsBool(TEXT("bPlayerInSight")))
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn)
        {
            UpdateLookState(ELookState::Investigating);
            BlackboardPtr->SetValueAsVector(TEXT("LastKnownPos"), PlayerPawn->GetActorLocation());

            BlackboardPtr->SetValueAsBool(TEXT("bPlayerInSight"), false);
            BlackboardPtr->SetValueAsBool(TEXT("bInvestigate"), true);

            MoveToLocation(PlayerPawn->GetActorLocation());
        }
    }
}

/*
* 플레이어를 추적하는 상태에서 일정 시간이 지나면 조사 상태(Investigating)로 변경하는 함수
*/

void AAIControllerCustom::OnPlayerTrackingTimerFinished()
{
    if (CurrentLookState == ELookState::PlayerTracking)
    {
        UpdateLookState(ELookState::Investigating);
    }
}

/*
*  플레이어가 감지된 후 일정 시간 후에 반응하도록 타이머 설정
*/

void AAIControllerCustom::StartPlayerInSightTimer()
{
    if (!GetWorldTimerManager().IsTimerActive(PlayerInSightTimerHandle))
    {
        GetWorldTimerManager().SetTimer(PlayerInSightTimerHandle, this, &AAIControllerCustom::OnPlayerInSightTimerFinished, 2.0f, false);
    }
}

/*
* 플레이어를 추적하는 타이머를 시작하여 일정 시간 후에 조사상태로 전환
*/

void AAIControllerCustom::StartPlayerTrackingTimer()
{
    if (!GetWorldTimerManager().IsTimerActive(PlayerTrackingTimerHandle))
    {
        GetWorldTimerManager().SetTimer(PlayerTrackingTimerHandle, this, &AAIControllerCustom::OnPlayerTrackingTimerFinished, 5.0f, false);
    }
}

/*
* AI가 특정 위치를 볼 수 있는지 확인하는 함수
*/

bool AAIControllerCustom::IsInSight(const FVector& TargetLocation) const
{
    return IsInSightIgnoreAngle(TargetLocation, true);
}

/*
*  AI가 특정 위치를 감지할 수 있는지 확인하는 함수 (각도 무시 가능)
*/

bool AAIControllerCustom::IsInSightIgnoreAngle(const FVector& TargetLocation, bool bCheckAngle) const
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return false;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    UCharacterMovementComponent* PlayerMovement = PlayerPawn->FindComponentByClass<UCharacterMovementComponent>();

    FVector StartLocation = ControlledPawn->GetActorLocation();
    FVector DirectionToTarget = (TargetLocation - StartLocation);
    float DistanceToTarget = DirectionToTarget.Size();

    // 플레이어의 현재 이동 속도 기반으로 동적 감지 반경 설정
    float PlayerCurrentSpeed = PlayerMovement ? PlayerMovement->Velocity.Size() : 0.0f;
    float DynamicDetectionRadius = DetectionRadius * (1.0f + (PlayerCurrentSpeed / 1000.0f)); // 시야 거리를 적절한 기준 값으로 조정

    if (DistanceToTarget > DynamicDetectionRadius)
    {
        return false;
    }

    DirectionToTarget.Normalize();
    FVector ForwardVector = ControlledPawn->GetActorForwardVector();

    if (bCheckAngle)
    {
        float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
        float AngleToTarget = FMath::Acos(DotProduct);
        float AngleInDegrees = FMath::RadiansToDegrees(AngleToTarget);

        if (AngleInDegrees > MaxSightAngle)
        {
            return false;
        }
    }

    // 라인 트레이스로 시야 막힘 확인
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(ControlledPawn);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        TargetLocation,
        ECC_Visibility,
        QueryParams
    );

    return (!bHit || (HitResult.GetActor() && HitResult.GetActor() == Cast<AActor>(PlayerPawn)));
}


/*
*  AI를 (수동으로)강제로 조사 상태로 변경하는 함수
*/
void AAIControllerCustom::ForceInvestigate()
{
    UBlackboardComponent* BlackboardPtr = GetBlackboardComponent();
    if (!BlackboardPtr) return;

    // 모든 상태 초기화
    BlackboardPtr->SetValueAsBool(TEXT("bPlayerInSight"), false);
    BlackboardPtr->SetValueAsBool(TEXT("bPatrolling"), false);
    BlackboardPtr->SetValueAsBool(TEXT("bAlert"), false);
    BlackboardPtr->SetValueAsBool(TEXT("bSearching"), false);
    BlackboardPtr->SetValueAsBool(TEXT("bReturnToHome"), false);

    // 조사 상태 강제 설정
    BlackboardPtr->SetValueAsBool(TEXT("bInvestigate"), true);

    // 상태 업데이트
    UpdateLookState(ELookState::Investigating);

    // 마지막 위치로 이동
    FVector LastKnownPos = BlackboardPtr->GetValueAsVector(TEXT("LastKnownPos"));
    if (!LastKnownPos.IsZero())
    {
        UE_LOG(LogTemp, Warning, TEXT("ForceInvestigate: 마지막 위치(%s)로 강제 이동"), *LastKnownPos.ToString());
        MoveToLocation(LastKnownPos, -1.0f, true);
    }
}

/*
*  AI 감각 시스템이 업데이트될 때 호출하는 함수
*/

void AAIControllerCustom::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    UBlackboardComponent* BlackboardPtr = GetBlackboardComponent();
    if (!BlackboardPtr) return;

    AAICharacter* CurrentAICharacter = Cast<AAICharacter>(GetPawn());
    if (!CurrentAICharacter) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    // 감지 반경 기존보다 3배 확장하여 감지 테스트
    float ExtendedDetectionRadius = DetectionRadius * 3.0f;
    float Distance = FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());

    // 플레이어 감지 여부
    bool bPlayerDetected = Distance <= ExtendedDetectionRadius && IsInSightIgnoreAngle(PlayerPawn->GetActorLocation(), true);
    bool bWasPlayerInSight = BlackboardPtr->GetValueAsBool(TEXT("bPlayerInSight"));

    UE_LOG(LogTemp, Warning,
        TEXT("Perception Update: Distance=%.2f, ExtendedRadius=%.2f, Detected=%d, WasInSight=%d"),
        Distance, ExtendedDetectionRadius, bPlayerDetected ? 1 : 0, bWasPlayerInSight ? 1 : 0);

    if (bPlayerDetected)
    {
        BlackboardPtr->SetValueAsVector(TEXT("LastKnownPos"), PlayerPawn->GetActorLocation());
        BlackboardPtr->SetValueAsBool(TEXT("bPlayerInSight"), true);
        BlackboardPtr->SetValueAsObject(TEXT("Target"), PlayerPawn);

        StartPlayerInSightTimer();
        UpdateLookState(ELookState::Alert);
    }
    else if (bWasPlayerInSight)
    {
        // 감지 되었다가 사라진 경우
        GetWorldTimerManager().ClearTimer(PlayerInSightTimerHandle);

        FVector LastKnownPos = PlayerPawn->GetActorLocation();
        BlackboardPtr->SetValueAsVector(TEXT("LastKnownPos"), LastKnownPos);
        BlackboardPtr->SetValueAsBool(TEXT("bPlayerInSight"), false);
        BlackboardPtr->SetValueAsBool(TEXT("bInvestigate"), true);

        //순찰 상태로 이동
        FTimerHandle InvestigateTimeoutHandle;
        GetWorldTimerManager().SetTimer(InvestigateTimeoutHandle, [this, BlackboardPtr]()
            {
                BlackboardPtr->SetValueAsBool(TEXT("bInvestigate"), false);
                UpdateLookState(ELookState::Patrolling);
            }, 10.0f, false);

        UpdateLookState(ELookState::Investigating);

        MoveToLocation(LastKnownPos, -1.0f);
    }

    // 다른 AI 감지 로직 
    for (AActor* UpdatedActor : UpdatedActors)
    {
        AAICharacter* PerceivedAICharacter = Cast<AAICharacter>(UpdatedActor);
        if (PerceivedAICharacter)
        {
            // 동일 종족 AI는 건너뛰기
            if (PerceivedAICharacter->AIType == CurrentAICharacter->AIType)
            {
                continue;
            }

            float AIDistance = FVector::Dist(GetPawn()->GetActorLocation(), PerceivedAICharacter->GetActorLocation());

            // 다른 종족 AI 감지 로직
            if (AIDistance <= DetectionRadius * 1.5f)
            {
                bool bInSight = IsInSightIgnoreAngle(PerceivedAICharacter->GetActorLocation(), false);
                if (bInSight)
                {
                    UE_LOG(LogTemp, Warning, TEXT("다른 종족 AI 감지! 종족: %s"),
                        *UEnum::GetValueAsString(PerceivedAICharacter->AIType));

                    // 감지된 AI의 위치 및 상태 업데이트
                    FSenseData& SightData = SenseDataMap[ESenseType::Sight];
                    SightData.LastKnownLocation = PerceivedAICharacter->GetActorLocation();
                    SightData.bIsDetected = true;
                    SightData.LastDetectionTime = GetWorld()->GetTimeSeconds();

                    BlackboardPtr->SetValueAsVector(TEXT("LastKnownPos"), PerceivedAICharacter->GetActorLocation());
                    BlackboardPtr->SetValueAsBool(TEXT("bPlayerInSight"), true);
                    BlackboardPtr->SetValueAsObject(TargetKey, PerceivedAICharacter);

                    UpdateLookState(ELookState::PlayerTracking);
                    return;
                }
            }
        }
    }
}

/*
*  특정 감각 감지 정보 업데이트
*/

void AAIControllerCustom::UpdateSenseData(ESenseType SenseType, bool bDetected, const FVector& Location)
{
    if (SenseDataMap.Contains(SenseType))
    {
        FSenseData& CurrentSenseData = SenseDataMap[SenseType];
        CurrentSenseData.bIsDetected = bDetected;
        if (bDetected)
        {
            CurrentSenseData.LastKnownLocation = Location;
            CurrentSenseData.LastDetectionTime = GetWorld()->GetTimeSeconds();
        }
        UBlackboardComponent* BlackboardPtr = GetBlackboardComponent();
        if (BlackboardPtr)
        {
            switch (SenseType)
            {
            case ESenseType::Sight:
                BlackboardPtr->SetValueAsBool(TEXT("bPlayerInSight"), bDetected);
                if (bDetected)
                {
                    BlackboardPtr->SetValueAsVector(TEXT("LastKnownPos"), Location);
                }
                break;
            case ESenseType::Hearing:
                // 추가 청각 로직
                break;
            }
        }
        UpdateLookState(bDetected ? ELookState::PlayerTracking : ELookState::Investigating);
    }
}

/*
*  감각 데이터 조회
*/

FSenseData AAIControllerCustom::GetSenseData(ESenseType SenseType) const
{
    if (SenseDataMap.Contains(SenseType))
    {
        return SenseDataMap[SenseType];
    }
    return FSenseData();
}

/*
*  ai의 현재 시야 상태 업데이트
*/

void AAIControllerCustom::UpdateLookState(ELookState NewState)
{
    if (SenseDataMap.Contains(ESenseType::Sight))
    {
        FSenseData& SightData = SenseDataMap[ESenseType::Sight];
        SightData.CurrentLookState = NewState;
    }
    switch (NewState)
    {
    case ELookState::Idle:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Idle"));
        break;
    case ELookState::Patrolling:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Patrolling"));
        break;
    case ELookState::SweepingSurroundings:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Sweeping Surroundings"));
        break;
    case ELookState::Alert:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Alert"));
        break;
    case ELookState::PlayerTracking:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Player Tracking"));
        break;
    case ELookState::Investigating:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Investigating"));
        break;
    case ELookState::Searching:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Searching"));
        break;
    case ELookState::ReturnToHome:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Return To Home"));
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("AI Look State: Unknown"));
        break;
    }
}

/*
*  AI 감각 초기화
*/

void AAIControllerCustom::InitializeSenseData()
{
    SenseDataMap.Add(ESenseType::Sight, FSenseData());
    SenseDataMap.Add(ESenseType::Hearing, FSenseData());
}

/*
* 감각 데이터 상태 출력하는 디버그 함수
*/

void AAIControllerCustom::DebugLogSenseData(ESenseType SenseType) const
{
    if (SenseDataMap.Contains(SenseType))
    {
        const FSenseData& SenseData = SenseDataMap[SenseType];
        switch (SenseType)
        {
        case ESenseType::Sight:
            UE_LOG(LogTemp, Warning, TEXT("=== Sight Sense Data ==="));
            break;
        case ESenseType::Hearing:
            UE_LOG(LogTemp, Warning, TEXT("=== Hearing Sense Data ==="));
            break;
        }
        UE_LOG(LogTemp, Warning, TEXT("Detected: %s"), SenseData.bIsDetected ? TEXT("True") : TEXT("False"));
        UE_LOG(LogTemp, Warning, TEXT("Last Known Location: %s"), *SenseData.LastKnownLocation.ToString());
        UE_LOG(LogTemp, Warning, TEXT("Last Detection Time: %f"), SenseData.LastDetectionTime);
        UE_LOG(LogTemp, Warning, TEXT("Current Look State: %d"), static_cast<int32>(SenseData.CurrentLookState));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Sense Data found for the given type"));
    }
}

/*
*   시각 정보를 시각적으로 디버그하는 함수
*/

void AAIControllerCustom::DrawDebugSenseInfo()
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    // 시작 위치
    FVector StartLocation = ControlledPawn->GetActorLocation() + FVector(0, 0, 50);
    FVector ForwardVector = ControlledPawn->GetActorForwardVector();

    // 시야각 감지 및 감지 범위 설정
    float FOVAngle = MaxSightAngle * 0.5f;
    float VisionRange = DetectionRadius;

    // 시야 콘 그리기
    DrawDebugCone(GetWorld(), StartLocation, ForwardVector, VisionRange,
        FMath::DegreesToRadians(FOVAngle),
        FMath::DegreesToRadians(FOVAngle),
        32, FColor::Red, false, 0.1f);

    // 마지막 위치 구체 그리기
    const FSenseData& SightData = SenseDataMap[ESenseType::Sight];
    if (SightData.bIsDetected)
    {
        DrawDebugSphere(GetWorld(), SightData.LastKnownLocation, 50.0f, 12, FColor::Green, false, 0.1f);
    }
}

/*
*  AI가 순찰할 위치를 설정하는 함수
*/

void AAIControllerCustom::SetPatrolPos(const FVector& NewPatrolPos)
{
    if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
    {
        BlackboardComp->SetValueAsVector(PatrolPosKey, NewPatrolPos);
    }
}

/*
*  AI의 홈 위치를 설정하는 함수
*/
void AAIControllerCustom::SetHomePos(const FVector& NewHomePos)
{
    if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
    {
        BlackboardComp->SetValueAsVector(HomePosKey, NewHomePos);
    }
}

/*
*  AI의 순찰 위치 가져오는 함수
*/
FVector AAIControllerCustom::GetPatrolPos() const
{
    if (const UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
    {
        return BlackboardComp->GetValueAsVector(PatrolPosKey);
    }
    return FVector::ZeroVector;
}

/*
*   AI의 홈 위치를 가져오는 함수
*/
FVector AAIControllerCustom::GetHomePos() const
{
    if (const UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
    {
        return BlackboardComp->GetValueAsVector(HomePosKey);
    }
    return FVector::ZeroVector;
}