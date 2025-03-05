#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inventory/Inventory.h"
#include "InventoryObject.generated.h"

class AController;
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_GGF_API UInventoryObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InventoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* InventoryInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AItemDataManager> ItemDataManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AItemDataManager* ItemDataManager;


	void CreatePlayerInventory(AController* PlayerController);
	void CreateEnemyInventory(AController* PlayerController);
	void CreateCreatureInventory(AController* PlayerController, TArray<FAnimalLoot> LootData);
	void CreateChestInventory(AController* PlayerController);
	void AddItem(FItemData* ItemData, int32 ItemCnt = 1);
	void AddAllItem(TArray<FItemData*> ItemDatas);
	void SendAllItem();
	void AddLootItem(TArray<FAnimalLoot> LootData);
	bool GetThrowingItem(int32 Idx);
	void ReturnThrowingItem(int32 Idx);

	bool CheckIsEmpty();

public:
	UInventoryObject();
};
