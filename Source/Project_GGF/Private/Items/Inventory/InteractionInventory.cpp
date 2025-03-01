#include "Items/Inventory/InteractionInventory.h"
#include "Items/Inventory/InteractionItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Components/ListView.h"
#include "Components/ListViewBase.h"
#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Kismet/GameplayStatics.h"

UInteractionInventory::UInteractionInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInteractionInventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemList)
	{
		if (UListViewBase* ListViewBase = Cast<UListViewBase>(ItemList))
		{
			ListViewBase->OnEntryWidgetGenerated().AddUObject(this, &UInteractionInventory::HandleEntryGenerated);
		}
	}
}

void UInteractionInventory::RefreshInventory()
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

void UInteractionInventory::AddAllItem(TArray<FItemData*> ItemData)
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

FItemData* UInteractionInventory::SendItem()
{
	return nullptr;
}

TArray<FItemData*> UInteractionInventory::SendAllItem()
{
	ItemList->ClearListItems();

	return Items;
}

void UInteractionInventory::HandleEntryGenerated(UUserWidget& Widget)
{
	if (UInteractionItem* Entry = Cast<UInteractionItem>(&Widget))
	{
		// 버튼 클릭 이벤트 바인딩
		Entry->OnEntryButtonClicked.AddDynamic(this, &UInteractionInventory::HandleEntryClicked);
	}
}

void UInteractionInventory::HandleEntryClicked(UObject* ClickedItem)
{
	UItemUIObject* ItemObj = Cast<UItemUIObject>(ClickedItem);

	if (AProject_GGFCharacter* PlayerCharacter = Cast<AProject_GGFCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		PlayerCharacter->GetInventoryObject()->AddItem(ItemObj->ItemData);
	}
}

