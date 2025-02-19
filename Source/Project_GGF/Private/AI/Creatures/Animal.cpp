#include "AI/Creatures/Animal.h"
#include "AIControllerCustom.h"
#include "Character/Project_GGFCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

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

    AAIControllerCustom* AIController = Cast<AAIControllerCustom>(GetController());
    if (AIController)
    {
        BlackboardComponent = AIController->GetBlackboardComponent();
    }
}

void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAnimal::UpdateAttackState(bool bIsHit)
{
    if (!BlackboardComponent) return;

    if (bIsHit)
    {
        BlackboardComponent->SetValueAsBool(TEXT("bAttacked"), true); 
        BlackboardComponent->SetValueAsVector(TEXT("AttackerLocation"), GetActorLocation());  

        GetWorld()->GetTimerManager().SetTimer(
            AttackResetTimerHandle,
            this,
            &AAnimal::ResetAttackState,
            3.0f, 
            false
        );
    }
}

void AAnimal::ResetAttackState()
{
    if (!BlackboardComponent) return;

    BlackboardComponent->SetValueAsBool(TEXT("bAttacked"), false);
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
