#include "AI/NPC/BTTask_LootMove.h"
#include "AI/AIControllerCustom.h"
#include "AI/AICharacter.h"

UBTTask_LootMove::UBTTask_LootMove()
{
    NodeName = "Move To Loot";
    bNotifyTick = true; 
}

EBTNodeResult::Type UBTTask_LootMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AICharacter = Cast<AAICharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    LootLocation = AICharacter->GetLootLocation();
    if (LootLocation.IsZero()) return EBTNodeResult::Failed;

    AIController->MoveToLocation(LootLocation);

    return EBTNodeResult::InProgress;
}

void UBTTask_LootMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (FVector::Dist(AICharacter->GetActorLocation(), LootLocation) < 200.0f)
    {
        AICharacter->AddItemToInventory();
        AICharacter->ClearLootLocation();
        UE_LOG(LogTemp, Warning, TEXT("%s collected loot!"), *AICharacter->GetName());

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
