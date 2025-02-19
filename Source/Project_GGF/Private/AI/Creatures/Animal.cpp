#include "AI/Creatures/Animal.h"
#include "AIControllerCustom.h"
#include "Character/Project_GGFCharacter.h"

AAnimal::AAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

    AIControllerClass = AAIControllerCustom::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AAnimal::BeginPlay()
{
	Super::BeginPlay();
}

void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAnimal::UpdateAttackState(bool bIsHit)
{
}

void AAnimal::Attack(AActor* Target)
{
    ACharacter* CharacterTarget = Cast<ACharacter>(Target); 
    if (CharacterTarget)
    {
        UHealthComponent* HealthComp = CharacterTarget->FindComponentByClass<UHealthComponent>(); 
        if (HealthComp)
        {
            HealthComp->TakeDamage(this, EDamageType::Melee, 0.0f, 10); 
            UE_LOG(LogTemp, Warning, TEXT("AI가 %s를 공격함!"), *CharacterTarget->GetName());
        }
    }
}


void AAnimal::OnDeath()
{
    //현재 문제점 : 만약 캐릭터가 안죽여도 그냥 캐릭터한테 아이템 지급되어버림
    AProject_GGFCharacter* Player = Cast<AProject_GGFCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (Player)
    {
        for (const FAnimalLoot& Loot : LootTable)
        {
            FString ItemName = Loot.ItemData.ItemName; 
            int32 Quantity = Loot.Quantity;

            Player->AddItemToInventory(ItemName, Quantity);
        }
    }

    Destroy();
}
