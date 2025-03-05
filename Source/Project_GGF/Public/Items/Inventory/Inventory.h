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
	virtual void RefreshInventory();

	virtual void AddItem(FItemData* ItemData, int32 ItemCnt = 1);
	virtual void AddAllItem(TArray<FItemData*> ItemData);
	virtual void DeleteItem();
	virtual void UseItem();
	virtual FItemData* SendItem();
	virtual void SendAllItem();
	virtual bool GetIsEmpty();
	virtual bool GetItem(FName ItemName);
	virtual void ReturnItem(FName ItemName);


public:
	UInventory(const FObjectInitializer& ObjectInitializer);
};
