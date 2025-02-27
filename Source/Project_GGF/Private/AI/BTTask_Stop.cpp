#include "AI/BTTask_Stop.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIControllerCustom.h"
#include "AI/Manager/AIStateManager.h"

/*
* 
* AI가 즉시 멈추는 Task
* 
*/
UBTTask_Stop::UBTTask_Stop()
{
    NodeName = TEXT("Stop");
}

EBTNodeResult::Type UBTTask_Stop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    // 즉시 캐릭터 이동 멈춤
    AICharacter->GetCharacterMovement()->StopMovementImmediately();

    // 상태 업데이트 
    AAIControllerCustom* CustomAIController = Cast<AAIControllerCustom>(AIController);
    if (CustomAIController->GetStateManager())
    {
        CustomAIController->GetStateManager()->UpdateLookState(ELookState::Idle);
    }

    return EBTNodeResult::Succeeded;
}