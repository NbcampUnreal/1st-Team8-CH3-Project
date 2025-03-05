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
	TArray<FItemData*> Items;

public:
	UFUNCTION()
	virtual void RefreshInventory();

	virtual void AddItem(FItemData* ItemData, int32 ItemCnt = 1);
	virtual void AddAllItem(TArray<FItemData*> ItemData);
	UFUNCTION()
	virtual void DeleteItem();
	UFUNCTION()
	virtual void UseItem();
	virtual FItemData* SendItem();
	UFUNCTION()
	virtual void SendAllItem();
	UFUNCTION()
	virtual bool GetIsEmpty();
	UFUNCTION()
	virtual bool GetItem(FName ItemName);
	UFUNCTION()
	virtual void ReturnItem(FName ItemName);


public:
	UInventory(const FObjectInitializer& ObjectInitializer);
};
