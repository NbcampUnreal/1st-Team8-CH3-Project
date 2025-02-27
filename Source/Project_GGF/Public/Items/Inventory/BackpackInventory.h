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
	virtual void RefreshInventory() override;
	virtual void AddItem(FItemData* ItemData) override;
	virtual void AddAllItem(TArray<FItemData*> ItemData) override;

public:

	UBackpackInventory(const FObjectInitializer& ObjectInitializer);
};
