#include "AI/BT/BTTask_Sleep.h"
#include "AIController.h"
#include "AI/Creatures/Animal.h"

UBTTask_Sleep::UBTTask_Sleep()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Sleep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AICharacter = Cast<AAnimal>(AIController->GetPawn());
    if (AICharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }
     
    if (AICharacter->SleepMontage)
    {
        UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AICharacter->PlayAnimMontage(AICharacter->SleepMontage);
            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UBTTask_Sleep::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance->Montage_IsPlaying(AICharacter->SleepMontage))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
