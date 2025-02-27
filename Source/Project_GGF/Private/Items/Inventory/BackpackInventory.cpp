#include "Items/Inventory/BackpackInventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Items/Inventory/BackpackItem.h"

UBackpackInventory::UBackpackInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}



void UBackpackInventory::RefreshInventory()
{
	ItemList->ClearListItems();

	for (FItemData* item : Items)
	{
		UItemUIObject* UItem = NewObject<UItemUIObject>();
		UItem->ItemData = item;
		ItemList->AddItem(UItem);
	}

}

void UBackpackInventory::AddItem(FItemData* ItemData)
{
	if (!ItemList)
		return;

	bool IsOwend = false;

	for (int i = 0; i < Items.Num();i++)
	{
		if (Items[i]->ItemName == ItemData->ItemName)
		{
			Items[i]->ItemCnt++;
			IsOwend = true;
			break;
		}
	}

	if (!IsOwend)
	{
		UItemUIObject* UItem = NewObject<UItemUIObject>();
		UItem->ItemData = ItemData;
		ItemList->AddItem(UItem);

		Items.Add(ItemData);
	}

	RefreshInventory();
}

void UBackpackInventory::AddAllItem(TArray<FItemData*> ItemData)
{
	if (!ItemList)
		return;

	for (FItemData* Item : ItemData)
	{
		bool IsOwend = false;

		for (int i = 0; i < Items.Num();i++)
		{
			if (Items[i]->ItemName == Item->ItemName)
			{
				Items[i]->ItemCnt += Item->ItemCnt;
				IsOwend = true;
				break;
			}
		}

		if (!IsOwend)
		{
			UItemUIObject* UItem = NewObject<UItemUIObject>();
			UItem->ItemData = Item;
			ItemList->AddItem(UItem);

			Items.Add(Item);
		}
	}

	RefreshInventory();
}
