// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/Inventory.h"

UInventory::UInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UInventory::RefreshInventory()
{
}

void UInventory::AddItem(FItemData* ItemData, int32 ItemCnt)
{
}

void UInventory::AddAllItem(TArray<FItemData*> ItemData)
{
}

void UInventory::DeleteItem()
{
}

void UInventory::UseItem()
{
}

FItemData* UInventory::SendItem()
{
	return nullptr;
}

void UInventory::SendAllItem()
{
	return;
}

bool UInventory::GetIsEmpty()
{
	if (Items.Num() <= 0)
		return true;
	else
		return false;
}

bool UInventory::GetItem(FName ItemName)
{
	for (int i = 0; i < Items.Num();i++)
	{
		if (Items[i]->ItemID == ItemName )
		{
			if (Items[i]->Quantity > 0)
			{
				Items[i]->Quantity --;

				RefreshInventory();
				return true;
			}
			break;
		}
	}

	return false;
}

void UInventory::ReturnItem(FName ItemName)
{
	for (int i = 0; i < Items.Num();i++)
	{
		if (Items[i]->ItemID == ItemName)
		{
			Items[i]->Quantity++;
			break;
		}
	}
}

