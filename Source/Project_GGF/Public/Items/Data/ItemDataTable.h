// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

UENUM(BlueprintType)
enum class EItemDataType : uint8
{
	Healing     UMETA(DisplayName = "Healing Item"),
	Throwable   UMETA(DisplayName = "Throwable Item"),
	Ammo        UMETA(DisplayName = "Ammo"),
	Attachment  UMETA(DisplayName = "Weapon Attachment")
};

UENUM(BlueprintType)
enum class EHealingType : uint8
{
	HP     UMETA(DisplayName = "Healing Item"),
	Speed   UMETA(DisplayName = "Throwable Item"),
	stamina        UMETA(DisplayName = "Ammo"),
	Attachment  UMETA(DisplayName = "Weapon Attachment")
};

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemDataType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _SpawnChance;
};


USTRUCT(BlueprintType)
struct FThrowingItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;
};

USTRUCT(BlueprintType)
struct FHealingItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHealingType HealingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
};
