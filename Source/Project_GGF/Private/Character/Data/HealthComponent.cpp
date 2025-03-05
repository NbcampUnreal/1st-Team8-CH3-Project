#include "Project_GGF/Public/Character/Data/HealthComponent.h"
#include "Project_GGF/Public/Character/Data/RespawnComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Creatures/Animal.h"
#include "AI/NPC/GGFAICharacter.h"
#include "Character/Project_GGFCharacter.h"
#include "Character/Data/HealthData.h"
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
   
    if (IsDead())
    {
		OnDeath();
    }
    else
    {
        // 여기에 Character->OnHit(Attacker); 이런식으로 추후 부모클래스생기면 만들기
        if (AGGFAICharacterBase* AICharacter = Cast<AGGFAICharacterBase>(GetOwner()))
        {
            AICharacter->OnHit(Attacker);
        }
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

    // 죽은 위치 
    FVector DeathLocation = OwnerCharacter->GetActorLocation();
   
    HandleLootDrop(DeathLocation);
    HandleRespawn(OwnerCharacter);
}

void UHealthComponent::HandleLootDrop(const FVector& DeathLocation)
{
    AGGFAICharacter* AICharacter = Cast<AGGFAICharacter>(LastAttacker);
    AProject_GGFCharacter* PlayerCharacter = Cast<AProject_GGFCharacter>(LastAttacker);

    AGGFGameMode* GameMode = Cast<AGGFGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    if (AICharacter || PlayerCharacter)
    {
        AActor* DeadActor = GetOwner();

        // DeadActor -> InventoryObject 받아와서 InteractinoActor에 넘겨주기.

        if (AAnimal* Animal = Cast<AAnimal>(DeadActor)) // 죽은 대상이 동물
        {
            TArray<FAnimalLoot> Loot = Animal->GetLoot();
            GameMode->SpawnLootInteractionActor(DeathLocation, Loot);

            if (AICharacter)
            {
                AICharacter->SetLootLocation(DeathLocation); 
            }
        }
        else if (AProject_GGFCharacter* DeadPlayer = Cast<AProject_GGFCharacter>(DeadActor)) // 죽은 대상이 플레이어
        {
            TArray<FAnimalLoot> PCLoot = DeadPlayer->GetInventoryLoot();
            GameMode->SpawnLootInteractionActor(DeathLocation, PCLoot);

            if (AICharacter)
            {
                AICharacter->SetLootLocation(DeathLocation); 
            }
        }
        else if (AGGFAICharacter* DeadAI = Cast<AGGFAICharacter>(DeadActor)) // 죽은 대상이 AICharacter
        {
            TArray<FAnimalLoot> NPCLoot = DeadAI->GetInventoryLoot();
            GameMode->SpawnLootInteractionActor(DeathLocation, NPCLoot);
        }
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
