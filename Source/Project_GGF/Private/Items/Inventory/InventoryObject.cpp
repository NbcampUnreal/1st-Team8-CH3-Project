#include "Items/Inventory/InventoryObject.h"
#include "Kismet/GameplayStatics.h"
#include "Project_GGF/Public/Controller/CharacterController.h"

UInventoryObject::UInventoryObject()
{

}

void UInventoryObject::CreatePlayerInventory(AController* PlayerController)
{
	
	ACharacterController* CharacterController = Cast<ACharacterController>(PlayerController);
	//PlayerController->ShowBackpackInventoryUI();

	InventoryInstance = CharacterController->CreateBackpackInventory(InventoryClass);


	ItemDataManager = ItemDataManagerClass.GetDefaultObject();

	TArray<FItemData*> ItemData = ItemDataManager->GetAllItemData();
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);
	Inventory->AddAllItem(ItemData);

	Inventory->RefreshInventory();
}

void UInventoryObject::AddItem(FItemData* ItemData)
{
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);
	Inventory->AddItem(ItemData);
}

void UInventoryObject::SendAllItem()
{
}
