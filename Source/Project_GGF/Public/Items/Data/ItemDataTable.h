// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AnimalLoot.h"
#include "ItemDataTable.generated.h"

UENUM(BlueprintType)
enum class EItemDataType : uint8
{
	Healing     UMETA(DisplayName = "Healing Item"),
	Throwable   UMETA(DisplayName = "Throwable Item"),
	Ammo        UMETA(DisplayName = "Ammo"),
	Attachment  UMETA(DisplayName = "Weapon Attachment"),
	Loot		UMETA(DisplayName = "Loot"),
};

UENUM(BlueprintType)
enum class EHealingType : uint8
{
	HP     UMETA(DisplayName = "HP"),
	Speed   UMETA(DisplayName = "Speed"),
	Stamina        UMETA(DisplayName = "Stamina"),
	DebuffRemoval  UMETA(DisplayName = "DebuffRemoval")
};

//UENUM(BlueprintType)
//enum class EAnimalType : uint8
//{
//	HP     UMETA(DisplayName = "HP"),
//	Speed   UMETA(DisplayName = "Speed"),
//	Stamina        UMETA(DisplayName = "Stamina"),
//	DebuffRemoval  UMETA(DisplayName = "DebuffRemoval")
//};

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemDataType EItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconTexture;
};



USTRUCT(BlueprintType)
struct FThrowingItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
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

//USTRUCT(BlueprintType)
//struct FAnimalLootData : public FTableRowBase
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FName ItemID;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	EAnimalType Animal;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float DropChance;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 MaxQuantity;
//};

