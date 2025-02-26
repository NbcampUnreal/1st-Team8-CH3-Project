#include "BTTask_Look.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllerCustom.h"

UBTTask_Look::UBTTask_Look()
{
    NodeName = TEXT("Look");
    bNotifyTick = false;
}

void UBTTask_Look::OnTargetLost(UBlackboardComponent* BlackboardComp)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(TEXT("LastKnownPos"), CurrentTarget->GetActorLocation());
        BlackboardComp->SetValueAsBool(TEXT("bInvestigate"), true);
        BlackboardComp->SetValueAsBool(TEXT("bPlayerInSight"), false);

        AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(CurrentAIController);
        if (CustomAIController)
        {
            CustomAIController->UpdateLookState(ELookState::Investigating);
        }
    }
}

EBTNodeResult::Type UBTTask_Look::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CurrentAIController = OwnerComp.GetAIOwner();
    CurrentOwnerComp = &OwnerComp;
    CurrentBlackboard = OwnerComp.GetBlackboardComponent();

    if (!CurrentAIController || !CurrentBlackboard)
    {
        return EBTNodeResult::Failed;
    }

    CurrentTarget = Cast<AActor>(CurrentBlackboard->GetValueAsObject(AAIControllerCustom::TargetKey));
    if (!CurrentTarget)
    {
        return EBTNodeResult::Failed;
    }

    CurrentBlackboard->SetValueAsVector(TEXT("LastKnownPos"), CurrentTarget->GetActorLocation());

    CurrentBlackboard->SetValueAsBool(TEXT("bPlayerInSight"), true);
    CurrentBlackboard->SetValueAsBool(TEXT("bInvestigate"), false);
    CurrentBlackboard->SetValueAsBool(TEXT("bPatrolling"), false);
    CurrentBlackboard->SetValueAsBool(TEXT("bAlert"), true);
    CurrentBlackboard->SetValueAsBool(TEXT("bSearching"), false);
    CurrentBlackboard->SetValueAsBool(TEXT("bReturnToHome"), false);
    CurrentBlackboard->SetValueAsBool(TEXT("bSweepingSurroundings"), false);

    AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(CurrentAIController);
    if (CustomAIController)
    {
        CustomAIController->UpdateLookState(ELookState::Alert);
    }

    FTimerHandle* RotationTimerHandle = new FTimerHandle();
    FTimerHandle* MovementTimerHandle = new FTimerHandle();

    CurrentAIController->GetWorldTimerManager().SetTimer(
        *RotationTimerHandle,
        [this, RotationTimerHandle, MovementTimerHandle]()
        {
            if (!IsValid(CurrentAIController) || !IsValid(CurrentTarget))
            {
                if (CurrentAIController)
                {
                    CurrentAIController->GetWorldTimerManager().ClearTimer(*RotationTimerHandle);
                    CurrentAIController->GetWorldTimerManager().ClearTimer(*MovementTimerHandle);
                }

                delete RotationTimerHandle;
                delete MovementTimerHandle;
                return;
            }

            APawn* ControlledPawn = CurrentAIController->GetPawn();
            if (!ControlledPawn)
            {
                CurrentAIController->GetWorldTimerManager().ClearTimer(*RotationTimerHandle);
                delete RotationTimerHandle;
                return;
            }

            if (CurrentBlackboard)
            {
                CurrentBlackboard->SetValueAsVector(TEXT("LastKnownPos"), CurrentTarget->GetActorLocation());
            }

            //AI 타겟 방향으로 회전
            FVector DirectionToTarget = CurrentTarget->GetActorLocation() - ControlledPawn->GetActorLocation();
            DirectionToTarget.Normalize();

            FRotator DesiredRotation = DirectionToTarget.Rotation();
            FRotator CurrentRotation = ControlledPawn->GetActorRotation();

            FRotator NewRotation = FMath::RInterpTo(
                CurrentRotation,
                DesiredRotation,
                GetWorld()->GetDeltaSeconds(),
                5.0f
            );


            ControlledPawn->SetActorRotation(NewRotation);


            CurrentAIController->MoveToLocation(CurrentTarget->GetActorLocation(), -1.0f, true, true);
        },
        0.1f,
        true
    );

    CurrentAIController->GetWorldTimerManager().SetTimer(
        *MovementTimerHandle,
        [this, RotationTimerHandle, MovementTimerHandle]()
        {
            if (CurrentAIController)
            {
                CurrentAIController->GetWorldTimerManager().ClearTimer(*RotationTimerHandle);
                CurrentAIController->GetWorldTimerManager().ClearTimer(*MovementTimerHandle);
            }

            delete RotationTimerHandle;
            delete MovementTimerHandle;

            if (!CurrentAIController || !CurrentBlackboard || !CurrentTarget || !CurrentOwnerComp)
                return;

            CurrentBlackboard->SetValueAsVector(TEXT("LastKnownPos"), CurrentTarget->GetActorLocation());

            // 타겟을 계속 보고 있으면 Investigate true 유지
            if (CurrentBlackboard->GetValueAsBool(TEXT("bPlayerInSight")))
            {
                CurrentBlackboard->SetValueAsBool(TEXT("bInvestigate"), true);
            }
            else
            {
                CurrentBlackboard->SetValueAsBool(TEXT("bInvestigate"), false);
            }

            AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(CurrentAIController);
            if (CustomAIController)
            {
                CustomAIController->UpdateLookState(ELookState::Investigating);
            }

            this->FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
        },
        WaitDuration,
        false
    );

    return EBTNodeResult::InProgress;
}