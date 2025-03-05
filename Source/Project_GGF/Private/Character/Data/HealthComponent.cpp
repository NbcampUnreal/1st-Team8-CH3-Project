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
#include "Interact/DeadAIItemsInteractiveActor.h"
#include "Items/Inventory/InventoryObject.h"

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
        if (AGGFCharacterBase* Character = Cast<AGGFCharacterBase>(GetOwner()))
        {
            Character->OnHit(Attacker);
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

    AGGFCharacterBase* OwnerCharacter = Cast<AGGFCharacterBase>(GetOwner());
    if (!OwnerCharacter) return;
    
    
    FVector DeathLocation = OwnerCharacter->GetActorLocation();
    
    OwnerCharacter->OnDie();
    FTimerHandle DieTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
           DieTimerHandle, 
           [OwnerCharacter]()
           {
               if (OwnerCharacter && OwnerCharacter->GetMesh())
               {
                   OwnerCharacter->GetMesh()->SetSimulatePhysics(true);
                   OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
               }
           }, 
           1.0f,
           false
       );
    
    GetWorld()->GetTimerManager().SetTimer(
        DeathTimerHandle,
        [this, DeathLocation, OwnerCharacter]()
        {
            HandleLootDrop(DeathLocation);
            HandleRespawn(OwnerCharacter);
        },
        5.0f,  
        false  
    );
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
            GameMode->SpawnAiInteractiveActor(DeathLocation, DeadPlayer->InventoryObjectInstance);

            if (AICharacter)
            {
                AICharacter->SetLootLocation(DeathLocation); 
            }
        }
        else if (AGGFAICharacter* DeadAI = Cast<AGGFAICharacter>(DeadActor)) // 죽은 대상이 AICharacter
        {
            GameMode->SpawnAiInteractiveActor(DeathLocation, DeadAI->InventoryObjectInstance);
        }
    }
}

void UHealthComponent::HandleRespawn(ACharacter* OwnerCharacter)
{
    URespawnComponent* RespawnComp = OwnerCharacter->FindComponentByClass<URespawnComponent>();
    if (RespawnComp)
    {
        GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
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
