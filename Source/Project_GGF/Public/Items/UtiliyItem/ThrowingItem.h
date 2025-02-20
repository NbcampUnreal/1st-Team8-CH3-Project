// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/ItemBase.h"
#include "ThrowingItem.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECT_GGF_API AThrowingItem : public AItemBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UProjectileMovementComponent* ProjectileMovement;


public:	
	// Sets default values for this actor's properties
	AThrowingItem();
};
