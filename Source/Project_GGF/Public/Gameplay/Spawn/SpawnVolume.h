#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class PROJECT_GGF_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();
	virtual void BeginPlay() override;
	void SpawnGroup();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ACharacter> GroupSpawnClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	int32 GroupSize;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawn")
	class UBoxComponent* SpawnBox;

private:
	FVector GetRandomPointInVolume();
};
