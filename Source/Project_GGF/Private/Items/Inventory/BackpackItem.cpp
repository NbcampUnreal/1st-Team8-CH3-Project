// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/BackpackItem.h"
#include "Items/Inventory/Test/ItemDataAsset.h"

UBackpackItem::UBackpackItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}




void UBackpackItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UItemUIObject* UiItem = Cast<UItemUIObject>(ListItemObject);
	ItemName->SetText(FText::FromString(UiItem->ItemData->ItemName.ToString()));
	ItemCnt->SetText(FText::FromString(FString::FromInt(UiItem->ItemData->ItemCnt)));
	IconImg->SetBrushFromTexture(UiItem->ItemData->IconTexture, false);

}
