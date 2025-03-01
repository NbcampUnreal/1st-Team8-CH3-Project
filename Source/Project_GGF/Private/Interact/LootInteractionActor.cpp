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

	// �������� Inventory�� Character�� Inventory Open

	Character->AddLootToInventory(LootItems);
	Destroy();
}

void ALootInteractionActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAICharacter* AICharacter = Cast<AAICharacter>(OtherActor);
	if (AICharacter)
	{
		// AICharacter�� �κ��丮OBJ�� ������ �Ѱ��ֱ�.
		AICharacter->AddLootToInventory(LootItems);
		Destroy();
	}
}
