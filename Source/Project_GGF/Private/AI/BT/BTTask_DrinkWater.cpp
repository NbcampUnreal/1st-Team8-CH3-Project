#include "AI/BT/BTTask_DrinkWater.h"
#include "AI/GGFAIController.h"
#include "AI/Creatures/Animal.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

UBTTask_DrinkWater::UBTTask_DrinkWater()
{
    bNotifyTick = true;
    OriginalSightRadius = -1.0f;
    OriginalHearingRange = -1.0f;
}

EBTNodeResult::Type UBTTask_DrinkWater::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    AICharacter = Cast<AAnimal>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    // 소음 감지, 인식 능력 저하
    if (OriginalSightRadius < 0.0f)
    {
        OriginalSightRadius = AIController->SightConfig->SightRadius;
    }
    AIController->SightConfig->SightRadius = OriginalSightRadius * 0.5f;

    if (OriginalHearingRange < 0.0f)
    {
        OriginalHearingRange = AIController->HearingConfig->HearingRange;
    }
    AIController->HearingConfig->HearingRange = OriginalHearingRange * 0.5f;

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
        AIController->SightConfig->SightRadius = OriginalSightRadius;
        AIController->HearingConfig->HearingRange = OriginalHearingRange;
        
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
