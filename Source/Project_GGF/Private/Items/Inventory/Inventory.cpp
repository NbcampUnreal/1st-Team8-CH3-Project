// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/Inventory.h"

UInventory::UInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ItemDataManager(nullptr)
{
}


void UInventory::AddItem(FItemData* ItemData)
{
}

void UInventory::DeleteItem()
{
}

void UInventory::UseItem()
{
}

void UInventory::SendItem()
{
}

