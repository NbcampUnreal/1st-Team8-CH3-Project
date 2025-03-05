// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/Data/ItemDataTable.h"
#include "ItemDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FItemData& Other) const
	{
		return ItemID == Other.ItemID;
	}

	UPROPERTY(EditAnywhere)
	FName ItemID;

	UPROPERTY(EditAnywhere)
	FString ItemName; // 아이템 이름

	UPROPERTY(EditAnywhere)
	EItemDataType EItemType;

	UPROPERTY(EditAnywhere)
	int32 Quantity;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere)
	UTexture2D* IconTexture;

};

UCLASS()
class UItemUIObject : public UObject
{
	GENERATED_BODY()

public:
	FItemData* ItemData;
};



UCLASS()
class PROJECT_GGF_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FItemData> Items;

};
