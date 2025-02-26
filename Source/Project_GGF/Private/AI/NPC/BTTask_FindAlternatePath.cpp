#include "AI/NPC/BTTask_FindAlternatePath.h"
#include "AI/NPC/PoacherController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindAlternatePath::UBTTask_FindAlternatePath()
{
	NodeName = TEXT("Find Alternate Path");
}

EBTNodeResult::Type UBTTask_FindAlternatePath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APoacherController* PoacherController = Cast<APoacherController>(OwnerComp.GetAIOwner());
	if (!PoacherController) return EBTNodeResult::Failed;

	APawn* ControlledPawn = PoacherController->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(ControlledPawn->GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation NewLocation;
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIControllerCustom::HomePosKey);

	if (NavSystem->GetRandomReachablePointInRadius(Origin, 500.0f, NewLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIControllerCustom::PatrolPosKey, NewLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}