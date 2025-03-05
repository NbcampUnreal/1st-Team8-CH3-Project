
#pragma once

#include "CoreMinimal.h"
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
