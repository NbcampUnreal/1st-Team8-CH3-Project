// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory/Inventory.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InventoryEntry.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class PROJECT_GGF_API UInventoryEntry : public UInventory, public IUserObjectListEntry
{
	GENERATED_BODY()
	
private:
	 

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

};
