#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/Weapon/Weapon.h"
#include "WeaponManager.generated.h"

//class AWeapon;
class ACharacter;

UCLASS(Blueprintable, BlueprintType)
class PROJECT_GGF_API UWeaponManager : public UObject
{
	GENERATED_BODY()
	
private:
	int32 CurrentIdx;

	ACharacter* Owner;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AWeapon>> WeaponClasses;
	
	UPROPERTY(EditAnywhere)
	TArray<AWeapon*> Weapons;

public:
	bool Attack();
	bool Reload();
	AWeapon* ChangeWeapon(int32 _Idx);

	void AddWeapon(AActor* _Actor);

	void CreateWeapons(ACharacter* _Owner);
	
public:
	UWeaponManager();
	~UWeaponManager();

};
