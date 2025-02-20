// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project_GGFGameMode.generated.h"

UCLASS(minimalapi)
class AProject_GGFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProject_GGFGameMode();
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SpawnAIFromAllSpawnPoints();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TSubclassOf<class AAICharacter> AICharacterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TSubclassOf<class AAIControllerCustom> AIControllerClass;

	void SpawnAIAtLocation(FVector SpawnLocation, FRotator SpawnRotation);

};



