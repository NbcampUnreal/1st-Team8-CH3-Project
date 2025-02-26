#include "AI/NPC/PoacherController.h"
#include "AI/NPC/Poacher.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

APoacherController::APoacherController()
{

}

void APoacherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DetectObstacle();
}

void APoacherController::DetectObstacle()
{
    if (!GetBlackboardComponent() || !GetPawn()) return;
    ACharacter* AICharacter = Cast<ACharacter>(GetPawn());
    if (!AICharacter) return;

    FVector PawnLocation = AICharacter->GetActorLocation();
    float CapsuleHalfHeight = AICharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    float CapsuleRadius = AICharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

    FVector TraceStart = PawnLocation - FVector(0, 0, CapsuleHalfHeight - 10.0f);
    FVector ForwardVector = AICharacter->GetActorForwardVector();
    FVector TraceEnd = TraceStart + (ForwardVector * DetectiveDistance);

    float SweepHalfHeight = CapsuleHalfHeight * 0.5f;

    FHitResult Hit;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(AICharacter);  // AI 자기 자신은 감지하지 않도록 예외 처리

    bool bHit = GetWorld()->SweepSingleByChannel(
        Hit,
        TraceStart,
        TraceEnd,
        FQuat::Identity,
        ECC_GameTraceChannel1,  // 기존 ECC_Visibility 대신 새로 만든 ObstacleTrace 사용
        FCollisionShape::MakeCapsule(CapsuleRadius, SweepHalfHeight),
        CollisionParams
    );

    DrawDebugCapsule(GetWorld(), TraceStart, SweepHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, 1.0f);
    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            // 바닥(FloorObject)인지 확인하여 무시
            if (HitActor->ActorHasTag("Floor") || HitActor->GetName().Contains("Floor") ||
                HitActor->GetName().Contains("Landscape"))
            {
                UE_LOG(LogTemp, Warning, TEXT("Hit ground-like actor: %s -> ignoring obstacle"), *HitActor->GetName());
                GetBlackboardComponent()->SetValueAsBool(TEXT("ObstacleDetected"), false);
                GetBlackboardComponent()->SetValueAsBool(TEXT("ObstacleTooHigh"), false);
                return;
            }

            // 장애물 높이 계산 (BoundingBox 사용)
            FBox Bounds = HitActor->GetComponentsBoundingBox();
            float ObstacleHeight = Bounds.Max.Z - Bounds.Min.Z;

            GetBlackboardComponent()->SetValueAsFloat(TEXT("ObstacleHeight"), ObstacleHeight);
            
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Obstacle Height: %f"), *HitActor->GetName(), ObstacleHeight);

            if (ObstacleHeight <= MaxJumpHeight)
            {
                // 점프 가능
                GetBlackboardComponent()->SetValueAsBool(TEXT("ObstacleDetected"), true);
                GetBlackboardComponent()->SetValueAsBool(TEXT("ObstacleTooHigh"), false);
                UE_LOG(LogTemp, Warning, TEXT("Obstacle Detected - Jumpable"));
            }
            else
            {
                // 점프 불가능 (우회)
                GetBlackboardComponent()->SetValueAsBool(TEXT("ObstacleDetected"), false);
                GetBlackboardComponent()->SetValueAsBool(TEXT("ObstacleTooHigh"), true);
                UE_LOG(LogTemp, Warning, TEXT("Obstacle Too High"));
            }
        }
    }
}