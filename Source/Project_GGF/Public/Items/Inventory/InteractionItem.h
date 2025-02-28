// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InteractionItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntryButtonClicked, UObject*, ClickedItem);

UCLASS()
class PROJECT_GGF_API UInteractionItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* IconImg;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCnt;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ButtonText;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEntryButtonClicked OnEntryButtonClicked;

	UFUNCTION()
	void HandleButtonClicked();
public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void SendItem();

public:
	UInteractionItem(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
