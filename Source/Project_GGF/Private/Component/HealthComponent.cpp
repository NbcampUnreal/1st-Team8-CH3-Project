#include "Project_GGF/Public/Component/HealthComponent.h"
#include "Project_GGF/Public/Component/RespawnComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    MaxHealth = 100;
    CurrentHealth = MaxHealth;
}



void UHealthComponent::TakeDamage(AActor* Attacker, EAttackType AttackType, float StiffTime, int HealthAmount)
{
    if (!Attacker || HealthAmount <= 0 || IsDead()) return;

 
    CurrentHealth = FMath::Clamp(CurrentHealth - HealthAmount, 0, MaxHealth);
   
    
    if (StiffTime > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            StiffTimerHandle,
            this,
            &UHealthComponent::EndStiffTime,
            StiffTime,
            false
        );
    }

   
    if (IsDead())
    {
		OnDeath();
    }
}


void UHealthComponent::Heal(int HealAmount)
{
    if (HealAmount > 0 && !IsDead())
    {
        CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0, MaxHealth);
       
    }
}

void UHealthComponent::OnDeath()
{
    if (bIsDead) return; 
    bIsDead = true;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

   
    URespawnComponent* RespawnComp = OwnerCharacter->FindComponentByClass<URespawnComponent>();
    if (RespawnComp)
    {
        
        GetWorld()->GetTimerManager().SetTimer(
            RespawnComp->RespawnTimerHandle,
            RespawnComp,
            &URespawnComponent::Respawn,
            RespawnComp->RespawnTime,
            false);
    }

    
    APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
    if (PlayerController)
    {
        PlayerController->DisableInput(PlayerController);
    }

    
    OwnerCharacter->SetActorHiddenInGame(true);
    OwnerCharacter->SetActorEnableCollision(false);

}



