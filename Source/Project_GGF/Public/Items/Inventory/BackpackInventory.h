// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory/Inventory.h"
#include "Components/ListView.h"
#include "BackpackInventory.generated.h"


UCLASS()
class PROJECT_GGF_API UBackpackInventory : public UInventory
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;

public:
	void AddItem(FItemData* ItemData) override;
	
public:

	UBackpackInventory(const FObjectInitializer& ObjectInitializer);
};
