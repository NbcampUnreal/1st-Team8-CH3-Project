#include "Interact/LootInteractionActor.h"
#include "AI/AICharacter.h"
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

	// 소유중인 Inventory와 Character의 Inventory Open

	Character->AddLootToInventory(LootItems);
	Destroy();
}

void ALootInteractionActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAICharacter* AICharacter = Cast<AAICharacter>(OtherActor);
	if (AICharacter)
	{
		// AICharacter의 인벤토리OBJ에 데이터 넘겨주기.
		AICharacter->AddLootToInventory(LootItems);
		Destroy();
	}
}
