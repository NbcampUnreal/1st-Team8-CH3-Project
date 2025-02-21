#include "AI/Creatures/BTTask_DrinkWater.h"
#include "AI/AIControllerCustom.h"
#include "AI/Creatures/Animal.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

UBTTask_DrinkWater::UBTTask_DrinkWater()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_DrinkWater::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIControllerCustom* AIControllerCustom = Cast<AAIControllerCustom>(OwnerComp.GetAIOwner());
    if (AIControllerCustom == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AICharacter = Cast<AAnimal>(AIControllerCustom->GetPawn());
    if (AICharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // 소음 감지, 인식 능력 저하
    if (AIControllerCustom->SightConfig)
    {
        AIControllerCustom->SightConfig->SightRadius = 200.0f;
    }
    
    /* 현재 구현 안되어있으므로 추후 수정
    if (AIControllerCustom->HearingConfig)
    {
        AIControllerCustom->HearingConfig->HearingRange = 250.0f; 
    }*/

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
        AAIControllerCustom* AIControllerCustom = Cast<AAIControllerCustom>(OwnerComp.GetAIOwner());
        if (AIControllerCustom && AIControllerCustom->SightConfig)
        {
            AIControllerCustom->SightConfig->SightRadius = 400.0f; 
        }

        /*
        if (AIControllerCustom && AIControllerCustom->HearingConfig)
        {
            AIControllerCustom->HearingConfig->HearingRange = 500.0f;
        }*/

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
