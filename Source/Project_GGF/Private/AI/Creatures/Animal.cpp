#include "AI/Creatures/Animal.h"
#include "AIControllerCustom.h"
#include "Gameplay/GGFGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"

AAnimal::AAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

    AIControllerClass = AAIControllerCustom::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
    GameMode = Cast<AGGFGameMode>(GetWorld()->GetAuthGameMode());

    AAIControllerCustom* AIController = Cast<AAIControllerCustom>(GetController());
    if (AIController)
    {
        BlackboardComponent = AIController->GetBlackboardComponent();
    }
}

void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (GameMode)
    {
        UpdateTimeState();
    }
}


void AAnimal::UpdateTimeState()
{
    float TimeRatio = GameMode->GetGameTimeRatio();

    BlackboardComponent->SetValueAsBool(TEXT("bIsNight"), (TimeRatio >= 0.7f));
    BlackboardComponent->SetValueAsBool(TEXT("bIsEvening"), (TimeRatio > 0.4f && TimeRatio < 0.7f));
    BlackboardComponent->SetValueAsBool(TEXT("bIsDay"), (TimeRatio <= 0.4f));
}

void AAnimal::UpdateAttackState(bool bIsHit)
{
}


void AAnimal::OnDeath()
{
    /* 캐릭터 받아서 캐릭터에 additemtoinventory 함수 호출해서 그 안에서 quest의 UpdateQuestProgress 호출하도록 하기
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (Player)
    {
        for (const FAnimalLoot& Loot : LootTable)
        {
            FString ItemName = Loot.ItemData.ItemName; 
            int32 Quantity = Loot.Quantity;

            Player->AddItemToInventory(ItemName, Quantity);
        }
    }*/

    Destroy();
}
