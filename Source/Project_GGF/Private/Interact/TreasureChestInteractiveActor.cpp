#include "Interact/TreasureChestInteractiveActor.h"
#include "Items/Inventory/InventoryObject.h"
#include "Project_GGF/Public/Controller/CharacterController.h"


ATreasureChestInteractiveActor::ATreasureChestInteractiveActor()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);



}

void ATreasureChestInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	InventoryObjectInstance = Cast<UInventoryObject>(InventoryObjectPtr.GetDefaultObject());

	if (InventoryObjectInstance)
	{
		InventoryObjectInstance->CreatePlayerInventory(GetWorld()->GetFirstPlayerController());
	}
}
