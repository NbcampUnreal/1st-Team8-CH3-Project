#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpawnInfo.generated.h"

USTRUCT(BlueprintType)
struct FSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ACharacter> CharacterClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	int32 Count = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	int32 GroupCount = 0;
};
