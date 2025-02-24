#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnManager.h"
#include "GGFGameMode.generated.h"

UENUM(BlueprintType)
enum class ESpawnType : uint8
{
    Bear,
    Boar,
    DeerDoe,
    DeerStag,
    AICharacter
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int32 BearCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int32 BoarCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int32 DeerDoeCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int32 DeerStagCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int32 AICharacterCount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TMap<ESpawnType, TSubclassOf<ACharacter>> AIClasses;

private:
    void SpawnAI(ESpawnType SpawnType, int32 Count);

    UPROPERTY()
    ASpawnManager* SpawnManager;
};
