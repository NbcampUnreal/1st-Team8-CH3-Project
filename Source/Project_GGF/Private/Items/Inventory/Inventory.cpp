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

