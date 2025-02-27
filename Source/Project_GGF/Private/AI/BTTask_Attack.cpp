#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "AI/Creatures/Animal.h"

UBTTask_Attack::UBTTask_Attack()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AICharacter = Cast<AAnimal>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    if (AICharacter->AttackMontage)
    {
        UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AICharacter->PlayAnimMontage(AICharacter->AttackMontage);

            // 히트박스 활성화 (공격 시)
            AICharacter->SetAttackHitboxActive(true);

            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance->Montage_IsPlaying(AICharacter->AttackMontage))
    {
        AICharacter->SetAttackHitboxActive(false);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
