#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestItemData.generated.h"


UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common,
    Rare,
    Legend
};

USTRUCT(BlueprintType)
struct FQuestItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;  

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemRarity Rarity;  
};
