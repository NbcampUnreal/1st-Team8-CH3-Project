
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"  
#include "RespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GGF_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URespawnComponent();

	virtual void BeginPlay() override;

public:

	FVector RespawnLocation;
	FRotator RespawnRotation;
	float RespawnTime;

	FTimerHandle RespawnTimerHandle;
	FTimerHandle DeathCleanupTimerHandle;
public:
	void Spawn();
	void Respawn();
	void DestroyOwner();
};
