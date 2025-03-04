#include "Project_GGF/Public/Character/Data/HealthComponent.h"
#include "Project_GGF/Public/Character/Data/RespawnComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Creatures/Animal.h"
#include "AI/NPC/GGFAICharacter.h"
#include "Character/Project_GGFCharacter.h"
#include "Character/Data/HealthData.h"
#include "Character/Data/HitDeadComponent.h"
#include "Gameplay/GGFGameMode.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    LoadHealthData();
}

void UHealthComponent::TakeDamage(AActor* Attacker, EAttackType AttackType, float StiffTime, int HealthAmount)
{
    if (!Attacker || HealthAmount <= 0 || IsDead()) return;

    SetLastAttacker(Attacker);
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

    AGGFCharacterBase* Character = Cast<AGGFCharacterBase>(GetOwner());
    if (!Character) return;
   
    if (IsDead())
    {
        Character->OnDie();
        UE_LOG(LogTemp, Warning, TEXT("%s 죽음."), *Character->GetName());
        //HandleRespawn(Character);
    }
    else
    {
        Character->OnHit(Attacker);
        UE_LOG(LogTemp, Warning, TEXT("%s가 때림. %s 데미지 -%d"), *Attacker->GetName(), *Character->GetName(), HealthAmount);
    }
}


void UHealthComponent::Heal(int HealAmount)
{
    if (HealAmount > 0 && !IsDead())
    {
        CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0, MaxHealth);
       
    }
}


void UHealthComponent::HandleRespawn(ACharacter* OwnerCharacter)
{
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

void UHealthComponent::LoadHealthData()
{
    AGGFGameMode* GameMode = Cast<AGGFGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;

    AActor* Owner = GetOwner();
    if (!Owner)
        return;

    ECharacterType CharacterType = GameMode->GetCharacterType(Owner->GetClass());

    FHealthData* Data = GameMode->GetCharacterStat(CharacterType);
    if (Data)
    {
        MaxHealth = Data->MaxHealth;
        CurrentHealth = MaxHealth;
        UE_LOG(LogTemp, Warning, TEXT("%s MaxHealth : %d"), *Owner->GetName(), Data->MaxHealth);
    }
}
