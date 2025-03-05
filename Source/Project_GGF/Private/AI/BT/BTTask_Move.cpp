#include "AI/BT/BTTask_Move.h"
#include "AI/GGFAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_Move::UBTTask_Move()
{
	NodeName = TEXT("Move");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Failed;

	ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
	if (!AICharacter) return EBTNodeResult::Failed;
	
	OriginalSpeed = AICharacter->GetCharacterMovement()->MaxWalkSpeed;
	AICharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetKey.SelectedKeyName);
	
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(
		TargetLocation,
		100.0f, 
		true,   
		true,  
		false,  
		true   
	);

	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_Move::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
    
	UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent();
	if (!PathComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
    
	if (PathComp->DidMoveReachGoal())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Move::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	ACharacter* AICharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AICharacter)
	{
		AICharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
	}
}
