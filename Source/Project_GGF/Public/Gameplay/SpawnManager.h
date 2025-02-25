#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Landscape.h"
#include "SpawnManager.generated.h"

UCLASS()
class PROJECT_GGF_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
    bool IsValidSpawnLocation(FVector Location);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
    TArray<FVector> GridSpawnPoints;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    bool bShowDebug = true;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 GridSize = 20;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MaxSpawnHeight = -6500.0f;

    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Spawn")
    void GenerateSpawnPointsInEditor();

private:
    void GenerateSpawnPoints();
    FVector GetLandscapeSize();
    float GetHeight(const FVector& Location);

    ALandscape* Landscape = nullptr;
    FVector Origin;
    FVector Extent;
};
