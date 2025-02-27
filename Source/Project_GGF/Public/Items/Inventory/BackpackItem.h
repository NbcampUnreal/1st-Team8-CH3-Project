// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BackpackItem.generated.h"


UCLASS()
class PROJECT_GGF_API UBackpackItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UImage* IconImg;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCnt;

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UBackpackItem(const FObjectInitializer& ObjectInitializer);

};
