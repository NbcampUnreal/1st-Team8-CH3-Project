#include "AI/Creatures/BTTask_Sleep.h"
#include "AIController.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_Sleep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    if (AICharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    /* 
    if (SleepMontage)
    {
        AICharacter->PlayAnimMontage(SleepMontage);
        return EBTNodeResult::Succeeded;
    }*/ 

    return EBTNodeResult::Failed;
}
