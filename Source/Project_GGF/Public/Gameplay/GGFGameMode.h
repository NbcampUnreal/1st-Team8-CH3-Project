#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/Data/HealthData.h"
#include "Items/Data/AnimalLoot.h"
#include "Gameplay/Spawn/SpawnInfo.h"
#include "Interact/DeadAIItemsInteractiveActor.h"
#include "Items/Inventory/InventoryObject.h"
#include "GGFGameMode.generated.h"

class ALootInteractionActor;
class ASpawnManager;
class ASpawnVolume;

UENUM()
enum class ECharacterType
{
    Bear,
    Boar,
    DeerDoe,
    DeerStag,
    AICharacter,
    Character
};

UCLASS()
class PROJECT_GGF_API AGGFGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGGFGameMode();

    float GetGameTimeRatio() const;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void GameOver();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    float TotalGameTime = 900.0f; 
    float CurrentTime = 0.0f;

    FTimerHandle GameTimeHandle;
    void UpdateGameTime();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    TMap<ECharacterType, FSpawnInfo> SpawnInfoMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    TSubclassOf<ASpawnVolume> SpawnVolumeClass;

    UPROPERTY(EditAnywhere, Category = Data)
    UDataTable* CharacterStatTable;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADeadAIItemsInteractiveActor> AiInteractiveActorClass;

    FHealthData* GetCharacterStat(ECharacterType type);
    ECharacterType GetCharacterType(TSubclassOf<ACharacter> CharacterClass);

    UFUNCTION(BlueprintCallable, Category = "Loot")
    void SpawnLootInteractionActor(const FVector& Location, const TArray<FAnimalLoot>& Loot);

    UFUNCTION(BlueprintCallable)
    void SpawnAiInteractiveActor(const FVector& Location, UInventoryObject* InventoryObj);
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void HandleLootDrop(AActor* DeadActor, AActor* LastAttacker, const FVector& DeathLocation);

private:
    void SpawnAI(ECharacterType SpawnType, int32 Count, int32 GroupCount);

    UPROPERTY()
    ASpawnManager* SpawnManager;
    
    UPROPERTY(EditDefaultsOnly, Category = "Loot")
    TSubclassOf<ALootInteractionActor> LootInteractionClass;
};
