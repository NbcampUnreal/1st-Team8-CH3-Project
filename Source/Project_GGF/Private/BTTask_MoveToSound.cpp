#include "BTTask_MoveToSound.h"
#include "AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTTask_MoveToSound::UBTTask_MoveToSound()
{
	NodeName = (TEXT("MoveToSound"));
}

EBTNodeResult::Type UBTTask_MoveToSound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIControllerCustom* AIController = Cast<AAIControllerCustom>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		// 소리 위치를 가져옴
		FVector SoundLocation = AIController->GetBlackboardComponent()->GetValueAsVector(TEXT("SoundLocation"));
		// 소리 위치로 이동
		AIController->MoveToLocation(SoundLocation);
	}

	return EBTNodeResult::Succeeded;
}