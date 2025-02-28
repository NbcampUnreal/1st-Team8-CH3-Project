// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/Inventory.h"

UInventory::UInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UInventory::RefreshInventory()
{
}

void UInventory::AddItem(FItemData* ItemData)
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

TArray<FItemData*> UInventory::SendAllItem()
{
	TArray<FItemData*> EmptyArray;
	return EmptyArray;
}

