// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Project_GGF/Public/Items/Data/ItemDataTable.h"
#include "Items/Inventory/Test/ItemDataAsset.h"
#include "ItemDataManager.generated.h"

UCLASS(Blueprintable)
class PROJECT_GGF_API AItemDataManager : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* ThrowingItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* HealingItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* AnimalLootDataTable;

public:

	FThrowingItemDataTable* GetThrowingItemDataTable(FName ItemID)
	{
		if (!ThrowingItemDataTable) return nullptr;

		return ThrowingItemDataTable->FindRow<FThrowingItemDataTable>(ItemID, TEXT("Item Lookup"));
	}
	
	FHealingItemDataTable* GetHealingItemDataTable(FName ItemID)
	{
		if (!HealingItemDataTable) return nullptr;

		return HealingItemDataTable->FindRow<FHealingItemDataTable>(ItemID, TEXT("Item Lookup"));
	}

	FAnimalLootData* GetLootDataTable(FName ItemID)
	{
		if (!AnimalLootDataTable) return nullptr;

		return AnimalLootDataTable->FindRow<FAnimalLootData>(ItemID, TEXT("Item Lookup"));
	}
	FItemDataTable* GetItemDataTable(FName ItemID)
	{
		if (!ItemDataTable) return nullptr;

		return ItemDataTable->FindRow<FItemDataTable>(ItemID, TEXT("Item Lookup"));
	}

	FItemData* GetItemData(FName ItemID)
	{
		if (!ItemData) return nullptr;

		return ItemData->FindRow<FItemData>(ItemID, TEXT("Item Lookup"));
	}
public:	
	AItemDataManager();
};
