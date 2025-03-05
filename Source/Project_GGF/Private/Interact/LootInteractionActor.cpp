#include "Interact/LootInteractionActor.h"
#include "AI/NPC/GGFAICharacter.h"
#include "Character/Project_GGFCharacter.h"
#include "Items/Inventory/InventoryObject.h"
#include "Project_GGF/Public/Controller/CharacterController.h"


void ALootInteractionActor::BeginPlay()
{
	Super::BeginPlay();


}

void ALootInteractionActor::SetLootData(const TArray<FAnimalLoot>& NewLoot)
{
	InventoryObjectInstance = NewObject<UInventoryObject>(this, InventoryObjectPtr);

	if (InventoryObjectInstance)
	{
		InventoryObjectInstance->CreateCreatureInventory(GetWorld()->GetFirstPlayerController(), NewLoot);
	}
}

void ALootInteractionActor::InteractionKeyPressed(AActor* Actor)
{
	if (!Actor) return;

	ACharacterController* MyPlayerController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
	MyPlayerController->ShowInteractInventoryUI(Cast<UUserWidget>(InventoryObjectInstance));
	AProject_GGFCharacter* Character = Cast<AProject_GGFCharacter>(Actor);
	if (Character)
	{
		Character->EndInteract();
	}
}

void ALootInteractionActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	AGGFAICharacter* AICharacter = Cast<AGGFAICharacter>(OtherActor);
	if (AICharacter)
	{
		// AICharacter�� �κ��丮OBJ�� ������ �Ѱ��ֱ�.
		AICharacter->AddLootToInventory(LootItems);
		Destroy();
	}
}
