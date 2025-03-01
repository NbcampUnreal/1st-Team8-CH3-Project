#include "Interact/LootInteractionActor.h"
#include "AI/NPC/GGFAICharacter.h"
#include "Character/Project_GGFCharacter.h"

void ALootInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALootInteractionActor::SetLootData(const TArray<FAnimalLoot>& NewLoot)
{
	LootItems = NewLoot;
}

void ALootInteractionActor::InteractionKeyPressed(AProject_GGFCharacter* Character)
{
	if (!Character) return;

	Character->AddLootToInventory(LootItems);
	Destroy();
}

void ALootInteractionActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGGFAICharacter* AICharacter = Cast<AGGFAICharacter>(OtherActor);
	if (AICharacter)
	{
		AICharacter->AddLootToInventory(LootItems);
		Destroy();
	}
}
