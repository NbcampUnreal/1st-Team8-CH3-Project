#include "AI/Manager/AISenseManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllerCustom.h"
#include "AI/Manager/AIStateManager.h"
#include "AI/Manager/AISightHandler.h"

void UAISenseManager::Initialize(UBlackboardComponent* InBlackboard, AAIControllerCustom* InController)
{
    BlackboardComp = InBlackboard;
    AIController = InController;
    StateManager = InController->GetStateManager();

    // 감각 데이터 초기화
    SenseDataMap.Add(ESenseType::Sight, FSenseData());
    SenseDataMap.Add(ESenseType::Hearing, FSenseData());
}

// 특정 감각 감지 정보 업데이트
void UAISenseManager::UpdateSenseData(ESenseType SenseType, bool bDetected, const FVector& Location)
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

        if (BlackboardComp)
        {
            switch (SenseType)
            {
            case ESenseType::Sight:
                BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), bDetected);
                if (bDetected)
                {
                    BlackboardComp->SetValueAsVector(TEXT("LastKnownPos"), Location);
                }
                break;
            case ESenseType::Hearing:
                // 추가 청각 로직
                break;
            }
        }
        StateManager->UpdateLookState(bDetected ? ELookState::PlayerTracking : ELookState::Investigating);
    }
}

// 감각 데이터 조회
FSenseData UAISenseManager::GetSenseData(ESenseType SenseType) const
{
    if (SenseDataMap.Contains(SenseType))
    {
        return SenseDataMap[SenseType];
    }
    return FSenseData();
}

// 감각 데이터 상태 출력하는 디버그 함수
void UAISenseManager::DebugLogSenseData(ESenseType SenseType) const
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
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Sense Data found for the given type"));
    }
}

// 시각 정보를 시각적으로 디버그하는 함수
void UAISenseManager::DrawDebugSenseInfo()
{
    if (!AIController || !GetWorld()) return;

    // 시각 감각 디버깅 (시야각)
    if (AIController->GetSightHandler())
    {
        FVector AIPos = AIController->GetPawn()->GetActorLocation() + FVector(0, 0, 50);
        FVector ForwardVector = AIController->GetPawn()->GetActorForwardVector();
        float FOVAngle = AIController->GetSightHandler()->MaxSightAngle * 0.5f;
        float VisionRange = AIController->GetSightHandler()->DetectionRadius;

        // 시야 콘 그리기
        DrawDebugCone(GetWorld(), AIPos, ForwardVector, VisionRange,
            FMath::DegreesToRadians(FOVAngle), 
            FMath::DegreesToRadians(FOVAngle),
            32, FColor::Red, false, 0.1f);
    }

    // 마지막 위치 구체 그리기
    if (SenseDataMap.Contains(ESenseType::Sight) && SenseDataMap[ESenseType::Sight].bIsDetected)
    {
        DrawDebugSphere(GetWorld(), SenseDataMap[ESenseType::Sight].LastKnownLocation, 50.0f, 12, FColor::Green, false, 0.1f);
    }
}
