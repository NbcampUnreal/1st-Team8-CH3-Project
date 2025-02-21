#include "BTTask_StopAndLook.h"
#include "AIControllerCustom.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_StopAndLook::UBTTask_StopAndLook()
{
    NodeName = "StopAndLook";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StopAndLook::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTask_StopAndLook::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIControllerCustom* AIController = Cast<AAIControllerCustom>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
    if (!AICharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AActor* PlayerActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target"));
    bool bPlayerInSight = AIController->GetBlackboardComponent()->GetValueAsBool(TEXT("bPlayerInSight"));

    if (PlayerActor && bPlayerInSight)
    {
        AICharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

        FVector Direction = PlayerActor->GetActorLocation() - AICharacter->GetActorLocation();
        Direction.Z = 0.f;  
        FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
        AICharacter->SetActorRotation(TargetRot);
    }
    else
    {

        AICharacter->GetCharacterMovement()->bOrientRotationToMovement = true;


        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }

}