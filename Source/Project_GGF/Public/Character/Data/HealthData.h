#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HealthData.generated.h"

USTRUCT(BlueprintType)
struct FHealthData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    FString CharacterType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    int MaxHealth = 100;
};
