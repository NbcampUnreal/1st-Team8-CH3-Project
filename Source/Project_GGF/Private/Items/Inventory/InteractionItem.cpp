#include "Items/Inventory/InteractionItem.h"
#include "Items/Inventory/Test/ItemDataAsset.h"
#include "Items/UtiliyItem/ItemBase.h"
#include "Items/UtiliyItem/HealingItem.h"
#include "Blueprint/IUserObjectListEntry.h"


UInteractionItem::UInteractionItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInteractionItem::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInteractionItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UItemUIObject* UiItem = Cast<UItemUIObject>(ListItemObject);
	ItemName->SetText(FText::FromString(UiItem->ItemData->ItemName.ToString()));
	ItemCnt->SetText(FText::FromString(FString::FromInt(UiItem->ItemData->Quantity)));
	IconImg->SetBrushFromTexture(UiItem->ItemData->IconTexture, false);

	//NativeOnListItemObjectSet(GetListItem<UItemUIObject>());

	Button->OnClicked.Clear();
	Button->OnClicked.AddDynamic(this, &UInteractionItem::HandleButtonClicked);
}

void UInteractionItem::HandleButtonClicked()
{
	FItemData* ItemData = GetListItem<UItemUIObject>()->ItemData;

	if (ItemData->Quantity == 0)
		return;

	ItemData->Quantity--;

	NativeOnListItemObjectSet(GetListItem<UItemUIObject>());

	OnEntryButtonClicked.Broadcast(GetListItem<UItemUIObject>());
}

void UInteractionItem::SendItem()
{
}
