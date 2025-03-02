#include "AI/BT/BTTask_Look.h"
#include "AI/GGFAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Look::UBTTask_Look()
{
	NodeName = TEXT("Look");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Look::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CurrentTime = 0.0f;
	return EBTNodeResult::InProgress;
}

void UBTTask_Look::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;
	
	FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AGGFAIController::TargetKey);
	FVector AICharacterLocation = AIPawn->GetActorLocation();
	FRotator DesiredRotation = (TargetLocation - AICharacterLocation).Rotation();

	// 회전 보간
	FRotator CurrentRotation = AIPawn->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaSeconds, 5.0f);
	
	AIPawn->SetActorRotation(NewRotation);

	// LookDuration 음수로 설정하면 회전하면 종료, 양수로 설정하면 그 시간되면 종료하도록 구현
	if (LookDuration < 0.0f)
	{
		if (FMath::Abs(NewRotation.Yaw - DesiredRotation.Yaw) < 5.0f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else 
	{
		CurrentTime += DeltaSeconds;
		if (CurrentTime >= LookDuration)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

}
