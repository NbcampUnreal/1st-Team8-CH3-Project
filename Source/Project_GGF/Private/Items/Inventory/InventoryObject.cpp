#include "Items/Inventory/InventoryObject.h"
#include "Kismet/GameplayStatics.h"
#include "Project_GGF/Public/Controller/CharacterController.h"
#include "Kismet/KismetMathLibrary.h"


UInventoryObject::UInventoryObject()
{

}

void UInventoryObject::CreatePlayerInventory(AController* PlayerController)
{
	ACharacterController* CharacterController = Cast<ACharacterController>(PlayerController);
	InventoryInstance = CreateWidget<UUserWidget>(CharacterController, InventoryClass);

	ItemDataManager = ItemDataManagerClass.GetDefaultObject();

	TArray<FItemData*> ItemData = ItemDataManager->GetAllItemData();
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);
	Inventory->AddAllItem(ItemData);
}

void UInventoryObject::CreateEnemyInventory(AController* PlayerController)
{
	
	ACharacterController* CharacterController = Cast<ACharacterController>(PlayerController);
	InventoryInstance = CreateWidget<UUserWidget>(CharacterController, InventoryClass);

	ItemDataManager = ItemDataManagerClass.GetDefaultObject();

	TArray<FItemData*> ItemData = ItemDataManager->GetAllItemData();
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);

	int32 TotalItemCnt = UKismetMathLibrary::RandomIntegerInRange(1, 5);
	int32 ItemNum;

	for (int i = 0; i < TotalItemCnt; i++)
	{
		ItemNum = UKismetMathLibrary::RandomIntegerInRange(0, ItemData.Num() - 1);

		if (ItemData[i]->EItemType == EItemDataType::Loot)
			continue;

		Inventory->AddItem(ItemData[ItemNum]);
	}
}

void UInventoryObject::CreateCreatureInventory(AController* PlayerController, TArray<FAnimalLoot> LootData)
{
	ACharacterController* CharacterController = Cast<ACharacterController>(PlayerController);
	InventoryInstance = CreateWidget<UUserWidget>(CharacterController, InventoryClass);

	ItemDataManager = ItemDataManagerClass.GetDefaultObject();

	TArray<FItemData*> ItemData = ItemDataManager->GetAllItemData();
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);

	for (int i = 0; i < LootData.Num(); i++)
	{
		for (int j = 0; j<ItemData.Num(); j++)
		{
			if (ItemData[j]->EItemType != EItemDataType::Loot)
				continue;

			if (LootData[i].ItemID == ItemData[j]->ItemID)
			{
				Inventory->AddItem(ItemData[j], LootData[i].Quantity);
				break;
			}
		}
	}
}

void UInventoryObject::CreateChestInventory(AController* PlayerController)
{
	ACharacterController* CharacterController = Cast<ACharacterController>(PlayerController);
	InventoryInstance = CreateWidget<UUserWidget>(CharacterController, InventoryClass);

	ItemDataManager = ItemDataManagerClass.GetDefaultObject();

	TArray<FItemData*> ItemData = ItemDataManager->GetAllItemData();
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);

	int32 TotalItemCnt = UKismetMathLibrary::RandomIntegerInRange(1, 5);
	int32 ItemNum;

	for (int i = 0; i < TotalItemCnt; i++)
	{
		ItemNum = UKismetMathLibrary::RandomIntegerInRange(0, ItemData.Num() - 1);

		if (ItemData[i]->EItemType == EItemDataType::Loot)
			continue;

		Inventory->AddItem(ItemData[ItemNum]);
	}

	Inventory->RefreshInventory();
}

void UInventoryObject::AddItem(FItemData* ItemData, int32 ItemCnt)
{
	if (InventoryInstance)
	{
		UInventory* Inventory = Cast<UInventory>(InventoryInstance);
		Inventory->AddItem(ItemData, ItemCnt);
	}
	
}

void UInventoryObject::AddAllItem(TArray<FItemData*> ItemDatas)
{
	if (InventoryInstance)
	{
		UInventory* Inventory = Cast<UInventory>(InventoryInstance);
		Inventory->AddAllItem(ItemDatas);
	}
}

void UInventoryObject::SendAllItem()
{
}

void UInventoryObject::AddLootItem(TArray<FAnimalLoot> LootData)
{
	if (InventoryInstance)
	{
		TArray<FItemData*> ItemData = ItemDataManager->GetAllItemData();
		UInventory* Inventory = Cast<UInventory>(InventoryInstance);

		for (int i = 0; i < LootData.Num(); i++)
		{
			for (int j = 0; j < ItemData.Num(); j++)
			{
				if (ItemData[j]->EItemType != EItemDataType::Loot)
					continue;

				if (LootData[i].ItemID == ItemData[j]->ItemID)
				{
					Inventory->AddItem(ItemData[j], LootData[i].Quantity);
					break;
				}
			}
		}
	}
	
}

bool UInventoryObject::GetThrowingItem(int32 Idx)
{
	if (InventoryInstance)
	{
		UInventory* Inventory = Cast<UInventory>(InventoryInstance);
		return Inventory->GetItem(FName(*FString::FromInt(Idx)));
	}

	return false;
}

void UInventoryObject::ReturnThrowingItem(int32 Idx)
{
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);

	Inventory->ReturnItem(FName(*FString::FromInt(Idx)));
}

bool UInventoryObject::CheckIsEmpty()
{
	UInventory* Inventory = Cast<UInventory>(InventoryInstance);

	return Inventory->GetIsEmpty();
}


