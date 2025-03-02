#include "AI/BT/BTTask_Investigate.h"
#include "AI/GGFAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_Investigate::UBTTask_Investigate()
{
	NodeName = TEXT("Investigate");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Investigate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AGGFAIController::TargetKey);

	AIController->MoveToLocation(TargetLocation, -1.0f, true, true);

	return EBTNodeResult::InProgress;
}

void UBTTask_Investigate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AGGFAIController* AIController = Cast<AGGFAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	// 현재 목표 위치 가져오기
	FVector CurrentTargetLocation = Blackboard->GetValueAsVector(AGGFAIController::TargetKey);
	bool bIsTargetVisible = Blackboard->GetValueAsBool(TEXT("bSight1"));

	if (bIsTargetVisible)
	{
		// 목표가 계속 보이면 InvestigatePos를 최신 위치로 갱신
		AIController->MoveToLocation(CurrentTargetLocation, -1.0f, true, true);
	}
	else
	{
		// 목표가 사라졌으면 마지막 위치로 이동
		AIController->MoveToLocation(CurrentTargetLocation, -1.0f, true, true);
	}

	// 이동이 끝났고, 대상이 시야에서 사라졌다면 조사 완료 (Succeeded)
	if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle && !bIsTargetVisible)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	/*
	 * 일단 고민중... 애니메이션 UAICharacterAnimInstance 만들어서 거기서 하던가...
	// 애니메이션 인스턴스를 가져옴
	UAICharacterAnimInstance* AnimInstance = Cast<UAICharacterAnimInstance>(AIPawn->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		// 1. ️현재 애니메이션 블루프린트에서 계산된 머리 회전 값을 가져옴
		float HeadYawOffset = AnimInstance->HeadYawOffset;

		// 2️. AI Perception 시야와 동기화 (컨트롤러 회전 변경)
		FRotator NewControlRotation = AIController->GetControlRotation();
		NewControlRotation.Yaw += HeadYawOffset;
		AIController->SetControlRotation(NewControlRotation);
	}*/
}
