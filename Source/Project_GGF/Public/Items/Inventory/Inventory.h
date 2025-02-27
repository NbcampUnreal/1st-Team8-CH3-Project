// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project_GGF/Public/Items/Data/ItemDataTable.h"
#include "Items/Inventory/Test/ItemDataAsset.h"
#include "Project_GGF/Public/Items/Manager/ItemDataManager.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GGF_API UInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AItemDataManager> ItemDataManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AItemDataManager* ItemDataManager;

	TArray<FItemData*> Items;
	TArray<int32>	ItemCount;

public:
	virtual void AddItem(FItemData* ItemData);
	virtual void DeleteItem();
	virtual void UseItem();
	virtual void SendItem();



public:
	UInventory(const FObjectInitializer& ObjectInitializer);
};
