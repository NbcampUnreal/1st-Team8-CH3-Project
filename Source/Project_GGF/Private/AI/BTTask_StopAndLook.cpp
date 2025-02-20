#include "BTTask_StopAndLook.h"
#include "AIControllerCustom.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_StopAndLook::UBTTask_StopAndLook()
{
    NodeName = "StopAndLook";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StopAndLook::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTask_StopAndLook::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIControllerCustom* AIController = Cast<AAIControllerCustom>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
    if (!AICharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AActor* PlayerActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("Target"));
    bool bPlayerInSight = AIController->GetBlackboardComponent()->GetValueAsBool(TEXT("bPlayerInSight"));

    if (PlayerActor && bPlayerInSight)
    {
        // 플레이어를 포커스로 설정
        AIController->SetFocus(PlayerActor);

        // 플레이어 감지 시 자동 회전 기능 끄기
        if (AICharacter->GetCharacterMovement())
        {
            AICharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
        }

    }
    else
    {

        AIController->ClearFocus(EAIFocusPriority::Gameplay);
        if (AICharacter->GetCharacterMovement())
        {
            AICharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }

}