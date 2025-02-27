#include "Items/Inventory/BackpackInventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Items/Inventory/BackpackItem.h"

UBackpackInventory::UBackpackInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UBackpackInventory::AddItem(FItemData* ItemData)
{
	if (!ItemList)
		return;

	UItemUIObject* UItem = NewObject<UItemUIObject>();
	UItem->ItemData = ItemData;
	ItemList->AddItem(UItem);
}
