// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/BackpackItem.h"
#include "Items/Inventory/Test/ItemDataAsset.h"
#include "Items/UtiliyItem/ItemBase.h"
#include "Items/UtiliyItem/HealingItem.h"
#include "Blueprint/IUserObjectListEntry.h"

UBackpackItem::UBackpackItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBackpackItem::NativeConstruct()
{
	Super::NativeConstruct();

	//Button->OnClicked.AddDynamic(this, &UBackpackItem::HandleButtonClicked);

}


void UBackpackItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UItemUIObject* UiItem = Cast<UItemUIObject>(ListItemObject);
	ItemName->SetText(FText::FromString(UiItem->ItemData->ItemName));
	ItemCnt->SetText(FText::FromString(FString::FromInt(UiItem->ItemData->Quantity)));
	IconImg->SetBrushFromTexture(UiItem->ItemData->IconTexture, false);

	//NativeOnListItemObjectSet(GetListItem<UItemUIObject>());

	if(UiItem->ItemData->EItemType != EItemDataType::Healing)
		Button->SetVisibility(ESlateVisibility::Collapsed);
	else
	{
		Button->OnClicked.Clear();
		Button->OnClicked.AddDynamic(this, &UBackpackItem::UseItem);
	}

}

void UBackpackItem::UseItem()
{
	FItemData* ItemData = GetListItem<UItemUIObject>()->ItemData;

	if (ItemData->Quantity == 0)
		return;
	
	AHealingItem* HealingItem = Cast<AHealingItem>(ItemData->ItemClass.GetDefaultObject());
	if (HealingItem)
	{
		HealingItem->UseItem();

		ItemData->Quantity--;

		NativeOnListItemObjectSet(GetListItem<UItemUIObject>());
	}
}
