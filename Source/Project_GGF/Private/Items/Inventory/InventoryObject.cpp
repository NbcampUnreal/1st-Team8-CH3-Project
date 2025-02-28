#include "Items/Inventory/InventoryObject.h"
#include "Kismet/GameplayStatics.h"
#include "Project_GGF/Public/Controller/CharacterController.h"

UInventoryObject::UInventoryObject()
{

}

void UInventoryObject::CreatePlayerInventory(AController* PlayerController)
{
	
	ACharacterController* CharacterController = Cast<ACharacterController>(PlayerController);
	InventoryInstance = CharacterController->CreateBackpackInventory(InventoryClass);

	ItemDataManager = ItemDataManagerClass.GetDefaultObject();

	TArray<FItemData*> ItemData = ItemDataManager->GetAllItemData();
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);
	Inventory->AddAllItem(ItemData);
}

void UInventoryObject::CreateEnemyInventory(AController* PlayerController)
{
	// ������ ���������ؼ� Add�ϱ�.
}

void UInventoryObject::CreateCreatureInventory(AController* PlayerController)
{
	// ����ǰ ���������ؼ� Add�ϱ�.
}

void UInventoryObject::AddItem(FItemData* ItemData)
{
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);
	Inventory->AddItem(ItemData);
}

void UInventoryObject::SendAllItem()
{
}
