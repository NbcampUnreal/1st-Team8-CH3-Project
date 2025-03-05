#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Quest/QuestItemData.h"
#include "AnimalLoot.generated.h"

UENUM(BlueprintType)
enum class EAnimalType : uint8
{
    Bear,
    Boar,
    DeerDoe,
    DeerStag
};

USTRUCT(BlueprintType)
struct FAnimalLootData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    EAnimalType Animal; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    FName ItemID; // 전리품 이름

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    FString ItemName; // 아이템 이름

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    float DropChance; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    int32 MaxQuantity; 
};

USTRUCT(BlueprintType)
struct FAnimalLoot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    FName ItemID; // 아이템 이름

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    FString ItemName; // 아이템 이름
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    int32 Quantity; 
};