#include "AI/Creatures/BTTask_DrinkWater.h"
#include "AIController.h"
#include "AI/Creatures/Animal.h"

UBTTask_DrinkWater::UBTTask_DrinkWater()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_DrinkWater::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    if (AICharacter->DrinkMontage)
    {
        UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AICharacter->PlayAnimMontage(AICharacter->DrinkMontage);
            return EBTNodeResult::InProgress; 
        }
    }

    return EBTNodeResult::Failed;
}

void UBTTask_DrinkWater::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance->Montage_IsPlaying(AICharacter->DrinkMontage))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
