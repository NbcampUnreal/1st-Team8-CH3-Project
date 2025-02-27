#include "AI/Manager/AISightHandler.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/AIControllerCustom.h"
#include "AI/Manager/AIStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacter.h"
#include "AI/Manager/AISenseManager.h"

UAISightHandler::UAISightHandler()
{
	PrimaryComponentTick.bCanEverTick = false;

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    SightConfig->SightRadius = DetectionRadius;
    SightConfig->LoseSightRadius = DetectionRadius + 100.0f;
    SightConfig->PeripheralVisionAngleDegrees = MaxSightAngle;
    SightConfig->SetMaxAge(0.1f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 100.0f;

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
}

void UAISightHandler::Initialize(AAIControllerCustom* InController, UAIPerceptionComponent* InPerception)
{
    AIController = InController;
    SenseManager = AIController->GetSenseManager();
    StateManager = AIController->GetStateManager();
    BlackboardComp = AIController->GetBlackboardComponent();
    AIPerception = InPerception;

    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

    AIPerception->OnPerceptionUpdated.AddDynamic(this, &UAISightHandler::PerceptionUpdated);
}

/*
* AI 감각 시스템이 업데이트될 때 호출하는 함수
* 감각을 업데이트할 때 AI가 플레이어를 감지하는지 판단
* 플레이어의 위치를 블랙보드에 저장 (LastKnownPos)
* 감지되었으면 AI 상태를 Alert로 변경
*/
void UAISightHandler::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    if (!AIController || !SenseManager || !BlackboardComp) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn) return;

    // 플레이어 감지 여부
    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
    bool bPlayerDetected = Distance <= DetectionRadius && IsInSightIgnoreAngle(PlayerPawn->GetActorLocation(), false); // 다시 true로 고치기 나중에
    bool bWasPlayerInSight = BlackboardComp->GetValueAsBool(TEXT("bPlayerInSight"));

    UE_LOG(LogTemp, Warning,
        TEXT("Perception Update: Distance=%.2f, Detected=%d, WasInSight=%d"),
        Distance, bPlayerDetected ? 1 : 0, bWasPlayerInSight ? 1 : 0);

    if (bPlayerDetected)
    {
        BlackboardComp->SetValueAsVector(TEXT("LastKnownPos"), PlayerPawn->GetActorLocation());
        BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), true);
        BlackboardComp->SetValueAsObject(TEXT("Target"), PlayerPawn);

        StateManager->UpdateLookState(ELookState::Alert);
        StartPlayerInSightTimer();
    }
    else if (bWasPlayerInSight)
    {
        // 감지 되었다가 사라진 경우
        GetWorld()->GetTimerManager().ClearTimer(PlayerInSightTimerHandle);

        FVector LastKnownPos = PlayerPawn->GetActorLocation();
        BlackboardComp->SetValueAsVector(TEXT("LastKnownPos"), LastKnownPos);
        BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), false);
        BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), true);

        //순찰 상태로 이동
        FTimerHandle InvestigateTimeoutHandle;
        GetWorld()->GetTimerManager().SetTimer(InvestigateTimeoutHandle, [this]()
            {
                BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), false);
                StateManager->UpdateLookState(ELookState::Patrolling);
            }, 10.0f, false);

        StateManager->UpdateLookState(ELookState::Investigating);
        AIController->MoveToLocation(LastKnownPos, -1.0f);
    }

    // 다른 AI 감지 로직 
    for (AActor* UpdatedActor : UpdatedActors)
    {
        AAICharacter* PerceivedAICharacter = Cast<AAICharacter>(UpdatedActor);
        if (PerceivedAICharacter)
        {
            // 동일 종족 AI는 건너뛰기
            if (PerceivedAICharacter->AIType == Cast<AAICharacter>(ControlledPawn)->AIType)
            {
                continue;
            }

            float AIDistance = FVector::Dist(ControlledPawn->GetActorLocation(), PerceivedAICharacter->GetActorLocation());

            // 다른 종족 AI 감지 로직
            if (AIDistance <= DetectionRadius * 1.5f)
            {
                bool bInSight = IsInSightIgnoreAngle(PerceivedAICharacter->GetActorLocation(), false);
                if (bInSight)
                {
                    UE_LOG(LogTemp, Warning, TEXT("다른 종족 AI 감지! 종족: %s"),
                        *UEnum::GetValueAsString(PerceivedAICharacter->AIType));

                    // 감지된 AI의 위치 및 상태 업데이트
                    FSenseData& SightData = SenseManager->SenseDataMap[ESenseType::Sight];
                    SightData.LastKnownLocation = PerceivedAICharacter->GetActorLocation();
                    SightData.bIsDetected = true;
                    SightData.LastDetectionTime = GetWorld()->GetTimeSeconds();

                    BlackboardComp->SetValueAsVector(TEXT("LastKnownPos"), PerceivedAICharacter->GetActorLocation());
                    BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), true);
                    BlackboardComp->SetValueAsObject(TEXT("Target"), PerceivedAICharacter);

                    StateManager->UpdateLookState(ELookState::PlayerTracking);
                    return;
                }
            }
        }
    }

}

// AI가 특정 위치를 볼 수 있는지 확인하는 함수
bool UAISightHandler::IsInSight(const FVector& TargetLocation) const
{
    return IsInSightIgnoreAngle(TargetLocation, true);
}


// AI가 목표를 직접 보고 있는지 확인함. (중간에 장애물이 있는지 없는지. bcheckangle을 false로 하면 시야각 상관없이 전부 체크 가능)
bool UAISightHandler::IsInSightIgnoreAngle(const FVector& TargetLocation, bool bCheckAngle) const
{
    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn) return false;

    FVector StartLocation = ControlledPawn->GetActorLocation();
    FVector DirectionToTarget = (TargetLocation - StartLocation);
    float DistanceToTarget = DirectionToTarget.Size();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    UCharacterMovementComponent* PlayerMovement = PlayerPawn->FindComponentByClass<UCharacterMovementComponent>();

    // 플레이어의 현재 이동 속도 기반으로 동적 감지 반경 설정
    float PlayerCurrentSpeed = PlayerMovement ? PlayerMovement->Velocity.Size() : 0.0f;
    float DynamicDetectionRadius = DetectionRadius * (1.0f + (PlayerCurrentSpeed / 1000.0f)); // 시야 거리를 적절한 기준 값으로 조정

    // 감지 거리 체크
    if (DistanceToTarget > DynamicDetectionRadius)
    {
        return false;
    }

    // AI가 목표를 바라보는지 체크 (시야각 확인)
    if (bCheckAngle)
    {
        DirectionToTarget.Normalize();
        FVector ForwardVector = ControlledPawn->GetActorForwardVector();

        float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
        float AngleToTarget = FMath::Acos(DotProduct);
        float AngleInDegrees = FMath::RadiansToDegrees(AngleToTarget);

        if (AngleInDegrees > MaxSightAngle)
        {
            return false; // AI의 시야각을 벗어나면 감지 불가
        }
    }

    // 라인 트레이스로 장애물 확인
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

    // AI와 플레이어 사이에 장애물이 없거나, 장애물이 플레이어인 경우 감지 성공
    return !bHit || (HitResult.GetActor() && HitResult.GetActor()->IsA<APawn>());
}

// 플레이어가 감지된 후 일정 시간 후에 반응하도록 타이머 설정
void UAISightHandler::StartPlayerInSightTimer()
{
    GetWorld()->GetTimerManager().SetTimer(
        PlayerInSightTimerHandle, this, &UAISightHandler::OnPlayerInSightTimerFinished, 2.0f, false);
}

// 플레이어가 ai시야에서 사라졌을 때, 일정 시간 후에 조사 상태 변경
void UAISightHandler::OnPlayerInSightTimerFinished()
{
    StateManager->HandlePlayerLost();
}



