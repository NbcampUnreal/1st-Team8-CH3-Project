#include "AI/BT/BTTask_LookAround.h"

#include "AI/GGFAIController.h"

UBTTask_LookAround::UBTTask_LookAround()
{
	NodeName = TEXT("Look Around");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	AccumulatedYaw = 0.0f;
	InitialYaw = AIPawn->GetActorRotation().Yaw;
	LastYaw = InitialYaw;
	RotationSpeed = 90.0f; // 90도/sec 속도로 회전 (4초 동안 360도 회전)

	return EBTNodeResult::InProgress;
}

void UBTTask_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	FRotator CurrentRotation = AIPawn->GetActorRotation();
	float NewYaw = FMath::FInterpConstantTo(CurrentRotation.Yaw, CurrentRotation.Yaw + 360.0f, DeltaSeconds, RotationSpeed);
	
	float DeltaYaw = FMath::FindDeltaAngleDegrees(LastYaw, NewYaw);
	AccumulatedYaw += FMath::Abs(DeltaYaw);

	AIPawn->SetActorRotation(FRotator(0.0f, NewYaw, 0.0f));
	LastYaw = NewYaw;
	
	if (AccumulatedYaw >= 360.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
