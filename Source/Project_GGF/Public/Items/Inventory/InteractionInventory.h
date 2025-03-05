// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory/Inventory.h"
#include "InteractionInventory.generated.h"

class UListView;
class UTextBlock;
class UInventoryObject;
class UButton;

UCLASS()
class PROJECT_GGF_API UInteractionInventory : public UInventory
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* TakeAllButton;
public:
	virtual void RefreshInventory() override;
	virtual void AddAllItem(TArray<FItemData*> ItemData) override;
	virtual FItemData* SendItem()override;
	virtual void SendAllItem()override;
	virtual void AddItem(FItemData* ItemData, int32 ItemCnt = 1) override;

	UFUNCTION()
	void HandleEntryTakeButtonClicked(UObject* ClickedItem);
	UFUNCTION()

	void HandleEntryTakeAllButtonClicked(UObject* ClickedItem);

	void HandleEntryGenerated(UUserWidget& Widget);

	UFUNCTION()

	void Exit();
public:
	UInteractionInventory(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
