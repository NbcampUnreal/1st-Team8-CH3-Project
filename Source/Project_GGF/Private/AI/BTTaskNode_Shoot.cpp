#include "AI/BTTaskNode_Shoot.h"
#include "AIControllerCustom.h"
#include "AI/AICharacter.h"

UBTTaskNode_Shoot::UBTTaskNode_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    AICharacter->Shoot();
    return EBTNodeResult::Succeeded;
}
