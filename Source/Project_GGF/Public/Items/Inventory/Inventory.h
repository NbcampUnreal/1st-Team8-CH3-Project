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

	virtual void AddItem(FItemData* ItemData);
	virtual void AddAllItem(TArray<FItemData*> ItemData);
	virtual void DeleteItem();
	virtual void UseItem();
	virtual FItemData* SendItem();
	virtual TArray<FItemData*> SendAllItem();
	


public:
	UInventory(const FObjectInitializer& ObjectInitializer);
};
