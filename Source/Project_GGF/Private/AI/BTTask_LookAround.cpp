#include "BTTask_LookAround.h"
#include "AIController.h"
#include "AI/AIControllerCustom.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LookAround::UBTTask_LookAround()
    : InitialRotation(FRotator::ZeroRotator)
    , TargetRotation(FRotator::ZeroRotator)
    , RotationDirection(0)
    , CurrentAIController(nullptr)
    , CurrentOwnerComp(nullptr)
{
    NodeName = TEXT("Look Around");
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CurrentAIController = OwnerComp.GetAIOwner();
    CurrentOwnerComp = &OwnerComp;
    if (!CurrentAIController)
        return EBTNodeResult::Failed;

    APawn* AIPawn = CurrentAIController->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    InitialRotation = AIPawn->GetActorRotation();

    TArray<float> RotationAngles;
    RotationAngles.Add(InitialRotation.Yaw - 90.0f);
    RotationAngles.Add(InitialRotation.Yaw + 90.0f);
    RotationAngles.Add(InitialRotation.Yaw - 180.0f);
    RotationAngles.Add(InitialRotation.Yaw);

    FTimerHandle RotationTimerHandle;
    FTimerHandle LookAroundTimerHandle;

    // 회전 보간 타이머
    CurrentAIController->GetWorldTimerManager().SetTimer(
        RotationTimerHandle,
        [this, AIPawn, RotationAngles]()
        {
            static int32 CurrentAngleIndex = 0;
            FRotator TargetRotation = FRotator(0, RotationAngles[CurrentAngleIndex], 0);
            FRotator CurrentRotation = AIPawn->GetActorRotation();

            FRotator NewRotation = FMath::RInterpTo(
                CurrentRotation,
                TargetRotation,
                GetWorld()->GetDeltaSeconds(),
                3.0f  
            );

            AIPawn->SetActorRotation(NewRotation);

            static float TimeInCurrentAngle = 0.0f;
            TimeInCurrentAngle += GetWorld()->GetDeltaSeconds();

            // 각 방향에 1초 이상 머무른 후 다음 각도로 이동
            if (TimeInCurrentAngle >= 1.0f)
            {
                CurrentAngleIndex = (CurrentAngleIndex + 1) % RotationAngles.Num();
                TimeInCurrentAngle = 0.0f;
            }
        },
        0.05f,  
        true
    );

    // 전체 Look Around 지속 시간 타이머
    CurrentAIController->GetWorldTimerManager().SetTimer(
        LookAroundTimerHandle,
        [this, &RotationTimerHandle, &LookAroundTimerHandle]()
        {
            if (CurrentAIController)
            {
                CurrentAIController->GetWorldTimerManager().ClearTimer(RotationTimerHandle);
                CurrentAIController->GetWorldTimerManager().ClearTimer(LookAroundTimerHandle);
            }

            if (CurrentOwnerComp)
            {
                UBlackboardComponent* BlackboardComp = CurrentOwnerComp->GetBlackboardComponent();
                if (BlackboardComp)
                {
                    BlackboardComp->SetValueAsBool(TEXT("bPatrolling"), true);
                }
                FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
            }
        },
        LookAroundDuration,
        false
    );
    return EBTNodeResult::InProgress;
}