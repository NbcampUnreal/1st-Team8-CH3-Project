#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECT_GGF_API AWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon/Component")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon/Component")
	UStaticMeshComponent* StaticMeshComp;

public:
	virtual void SpawnWeapon(FVector _Location, FRotator _Rotator);


public:	
	AWeapon();
};
