// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory/Inventory.h"
#include "ItemDataAsset.h"
#include "InventoryWidget.generated.h"

class UListView;

UCLASS()
class PROJECT_GGF_API UInventoryWidget : public UInventory
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;


public:
	void RefreshInventory(const TArray<FItemData>& Inventory);



};
