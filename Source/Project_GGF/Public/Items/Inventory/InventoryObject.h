// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inventory/Inventory.h"
#include "InventoryObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GGF_API UInventoryObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* InventoryInstance;


	void AddItem();
	void SendAllItem();

public:
	UInventoryObject();
};
