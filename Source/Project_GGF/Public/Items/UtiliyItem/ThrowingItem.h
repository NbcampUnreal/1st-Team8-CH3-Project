#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Camera/CameraShakeBase.h"

#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/UtilityItem.h"
#include "ThrowingItem.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


UCLASS()
class PROJECT_GGF_API AThrowingItem : public AUtilityItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item/Component")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item/Component")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item/Component")
	UStaticMeshComponent* StaticMeshComp;

	// ÀÌÆåÆ® ÄÄÆ÷³ÍÆ®------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item/Component")
	UNiagaraComponent* MuzzleNiagaraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item/Component")
	UNiagaraSystem* MuzzleNiagaraSys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item/Component")
	UAudioComponent* MuzzleAudioComp;

	// CameraShake
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item/Component")
	TSubclassOf<UCameraShakeBase> FireCameraShakeClass;

	UPROPERTY(VisibleAnywhere)
	float Range;

	UPROPERTY(VisibleAnywhere)
	float Time;

	UPROPERTY(VisibleAnywhere)
	float Damage;


	UPROPERTY(VisibleAnywhere)
	float Duration;

	bool bIsStartActive;

	FTimerHandle ActivationTimer;

protected:
	virtual void PlaySound();

	virtual void PlayVFX();

	virtual void PlayCameraShake();

	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent);


public:
	virtual void Throw(FVector LaunchVelocity);


protected:
	virtual void Activation();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AThrowingItem();
};
