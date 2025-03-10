#include "Items/Inventory/BackpackInventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Items/Inventory/BackpackItem.h"

UBackpackInventory::UBackpackInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBackpackInventory::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBackpackInventory::RefreshInventory()
{
	ItemList->ClearListItems();

	for (FItemData* item : Items)
	{
		if (item->Quantity < 0)
		{
			item->Quantity = 0;
		}
		UItemUIObject* UItem = NewObject<UItemUIObject>();
		UItem->ItemData = item;
		ItemList->AddItem(UItem);
	}

}

void UBackpackInventory::AddItem(FItemData* ItemData, int32 ItemCnt)
{
	if (!ItemList)
		return;


	for (int i = 0; i < Items.Num();i++)
	{
		if (Items[i]->ItemID == ItemData->ItemID)
		{
			Items[i]->Quantity += ItemCnt;

			RefreshInventory();
			return;
		}
	}

	UItemUIObject* UItem = NewObject<UItemUIObject>();
	UItem->ItemData = ItemData;
	ItemList->AddItem(UItem);

	Items.Add(ItemData);
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
			if (Items[i]->ItemID == Item->ItemID)
			{
				Items[i]->Quantity += Item->Quantity;
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

void UBackpackInventory::UseItem()
{
}



