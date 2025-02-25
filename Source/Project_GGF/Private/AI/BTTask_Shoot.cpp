#include "AI/BTTask_Shoot.h"
#include "AIControllerCustom.h"
#include "AI/AICharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = "Shoot";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AICharacter = Cast<AAICharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    if (AICharacter->FireMontage)
    {
        UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AICharacter->PlayAnimMontage(AICharacter->FireMontage);
            AICharacter->Shoot();
            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UBTTask_Shoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance->Montage_IsPlaying(AICharacter->FireMontage))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
