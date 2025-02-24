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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon/Component")
	USceneComponent* LeftHandSceneComp;

	bool bIsEquipped;

public:
	virtual void SpawnWeapon(FVector _Location, FRotator _Rotator);
	virtual void AttachWeaponToBack(USceneComponent* _SceneComp);
	
	virtual void AttachWeaponToHand(TArray<USceneComponent*> _SceneComp);

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
