#include "AI/NPC/BTTask_Jump.h"
#include "AI/NPC/Poacher.h"
#include "AI/NPC/PoacherController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Jump::UBTTask_Jump()
{
	NodeName = TEXT("Poacher Jump");
}

EBTNodeResult::Type UBTTask_Jump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APoacherController* PoacherController = Cast<APoacherController>(OwnerComp.GetAIOwner());
	if (!PoacherController) return EBTNodeResult::Failed;

	APoacher* Poacher = Cast<APoacher>(PoacherController->GetPawn());
	if (!Poacher) return EBTNodeResult::Failed;

	bool bObstacleDetected = PoacherController->GetBlackboardComponent()->GetValueAsBool(TEXT("ObstacleDetected"));

	if (bObstacleDetected && Poacher->CanJump())
	{
		float ObstacleHeight = PoacherController->GetBlackboardComponent()->GetValueAsFloat(TEXT("ObstacleHeight"));

		float JumpStrength = FMath::Clamp(ObstacleHeight / PoacherController->MaxJumpHeight, 0.5f, 1.5f);

		UE_LOG(LogTemp, Warning, TEXT("AI JUMPING WITH STRENGTH : %f"), JumpStrength);

		Poacher->LaunchCharacter(FVector(0, 0, JumpStrength * 600.0f), false, true);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

