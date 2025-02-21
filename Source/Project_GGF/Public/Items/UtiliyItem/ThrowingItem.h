
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


public:	
	AThrowingItem();
};
