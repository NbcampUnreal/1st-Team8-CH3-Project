#include "Items/Inventory/InteractionInventory.h"
#include "Items/Inventory/InteractionItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Components/ListView.h"
#include "Components/ListViewBase.h"
#include "Components/Button.h"
#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Project_GGF/Public/Controller/CharacterController.h"
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

	ExitButton->OnClicked.Clear();
	ExitButton->OnClicked.AddDynamic(this, &UInteractionInventory::Exit);

	TakeAllButton->OnClicked.Clear();
	TakeAllButton->OnClicked.AddDynamic(this, &UInteractionInventory::SendAllItem);
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

void UInteractionInventory::SendAllItem()
{
	if (AProject_GGFCharacter* PlayerCharacter = Cast<AProject_GGFCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		PlayerCharacter->GetInventoryObject()->AddAllItem(Items);
	}

	ItemList->ClearListItems();

	return;
}

void UInteractionInventory::AddItem(FItemData* ItemData, int32 ItemCnt)
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

void UInteractionInventory::HandleEntryGenerated(UUserWidget& Widget)
{
	if (UInteractionItem* Entry = Cast<UInteractionItem>(&Widget))
	{
		// 버튼 클릭 이벤트 바인딩
		Entry->OnEntryButtonClicked.AddDynamic(this, &UInteractionInventory::HandleEntryTakeButtonClicked);
	}
}

void UInteractionInventory::Exit()
{
	// PlayerController에서 UI끄기
	ACharacterController* MyPlayerController = Cast<ACharacterController>(GetOwningPlayer());
	if (MyPlayerController)
	{
		MyPlayerController->RemoveInteractInventoryUI(this);
	}
}

void UInteractionInventory::HandleEntryTakeButtonClicked(UObject* ClickedItem)
{
	UItemUIObject* ItemObj = Cast<UItemUIObject>(ClickedItem);

	if (AProject_GGFCharacter* PlayerCharacter = Cast<AProject_GGFCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		PlayerCharacter->GetInventoryObject()->AddItem(ItemObj->ItemData);

		if (ItemObj->ItemData->EItemType == EItemDataType::Loot)
		{
			PlayerCharacter->AddItemToInventory(ItemObj->ItemData->ItemName, 1);
		}
	}
}

void UInteractionInventory::HandleEntryTakeAllButtonClicked(UObject* ClickedItem)
{
	UItemUIObject* ItemObj = Cast<UItemUIObject>(ClickedItem);

	if (AProject_GGFCharacter* PlayerCharacter = Cast<AProject_GGFCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		PlayerCharacter->GetInventoryObject()->AddItem(ItemObj->ItemData, ItemObj->ItemData->Quantity);

		if (ItemObj->ItemData->EItemType == EItemDataType::Loot)
		{
			PlayerCharacter->AddItemToInventory(ItemObj->ItemData->ItemName, ItemObj->ItemData->Quantity);
		}
	}
}

