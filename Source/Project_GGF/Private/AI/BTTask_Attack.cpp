#include "AI/BTTask_Attack.h"
#include "AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/HealthComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
    if (!Target) return EBTNodeResult::Failed;

    ACharacter* CharacterTarget = Cast<ACharacter>(Target);
    if (CharacterTarget)
    {
        UHealthComponent* HealthComp = CharacterTarget->FindComponentByClass<UHealthComponent>();
        if (HealthComp)
        {
            HealthComp->TakeDamage(AICharacter, EDamageType::Melee, 0.0f, 10);
            UE_LOG(LogTemp, Warning, TEXT("AI가 %s를 공격함!"), *CharacterTarget->GetName());

            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
