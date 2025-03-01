// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory/Inventory.h"
#include "InteractionInventory.generated.h"

class UListView;
class UTextBlock;
class UInventoryObject;

UCLASS()
class PROJECT_GGF_API UInteractionInventory : public UInventory
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;
	
public:
	virtual void RefreshInventory() override;
	virtual void AddAllItem(TArray<FItemData*> ItemData) override;
	virtual FItemData* SendItem()override;
	virtual TArray<FItemData*> SendAllItem()override;

	UFUNCTION()
	void HandleEntryClicked(UObject* ClickedItem);

	void HandleEntryGenerated(UUserWidget& Widget);
public:
	UInteractionInventory(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
