#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;

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
	virtual void AttachWeaponToBack(USkeletalMeshComponent* _SceneComp, FName _BomeName);
	virtual void AttachWeaponToHand(USkeletalMeshComponent* CharacterMesh, FName HandSocketName);
	virtual void AttachWeaponToSocket(USkeletalMeshComponent* CharacterMesh, FName HandSocketName, FName WeaponSocketName);
	void HideWeapon();
	void ShowWeapon();


public:
	AWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
