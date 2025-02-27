#include "AI/GGFAICharacterBase.h"
#include "AI/AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"

void AGGFAICharacterBase::BeginPlay()
{
    Super::BeginPlay();

    AAIControllerCustom* AIController = Cast<AAIControllerCustom>(GetController());
    if (AIController)
    {
        BlackboardComponent = AIController->GetBlackboardComponent();
    }
}

void AGGFAICharacterBase::OnHit(AActor* Attacker)
{
    FVector AttackerLocation = Attacker->GetActorLocation();
    UpdateAttackState(true, AttackerLocation);
}

void AGGFAICharacterBase::OnDie()
{
}

// 맞으면 bAttacked true로 변경 + AttackerLocation 
void AGGFAICharacterBase::UpdateAttackState(bool bIsHit, const FVector& AttackerLocation)
{
    if (!BlackboardComponent) return;

    if (bIsHit)
    {
        BlackboardComponent->SetValueAsBool(TEXT("bAttacked"), true);
        BlackboardComponent->SetValueAsVector(TEXT("AttackerLocation"), AttackerLocation);

        GetWorld()->GetTimerManager().SetTimer(
            AttackResetTimerHandle,
            this,
            &AGGFAICharacterBase::ResetAttackState,
            5.0f,
            false
        );
    }
}

void AGGFAICharacterBase::ResetAttackState()
{
    if (!BlackboardComponent) return;

    BlackboardComponent->SetValueAsBool(TEXT("bAttacked"), false);
}
