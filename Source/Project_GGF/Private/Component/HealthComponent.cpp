#include "Project_GGF/Public/Component/HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    MaxHealth = 100;
    CurrentHealth = MaxHealth;
}



void UHealthComponent::TakeDamage(ACharacter* Attacker, EDamageType DamageType, float StiffTime, int HealthAmount)
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
       
    }
}


void UHealthComponent::Heal(int HealAmount)
{
    if (HealAmount > 0 && !IsDead())
    {
        CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0, MaxHealth);
       
    }
}


int UHealthComponent::GetCurrentHealth()
{ return CurrentHealth; }

bool UHealthComponent::IsDead() 
{ return CurrentHealth <= 0; } 

void UHealthComponent::EndStiffTime()
{
    StiffTimerHandle.Invalidate();
}