#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/Weapon/Weapon.h"
#include "TimerManager.h"
#include "WeaponManager.generated.h"

//class AWeapon;
class ACharacter;
class AThrowingItem;

UCLASS(Blueprintable, BlueprintType)
class PROJECT_GGF_API UWeaponManager : public UObject
{
	GENERATED_BODY()

private:
	int32 CurrentIdx;
	int32 MaxIdx;
	FTimerHandle TimerHandle;
	
	ACharacter* Owner;
public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

	TArray<AWeapon*> Weapons;

	UPROPERTY(EditAnywhere)
	TArray<int32> ItemID;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AThrowingItem>> ThrowingItemClasses;

	TArray<AThrowingItem*> ThrowingItems;

public:
	bool Attack();
	bool Reload();
	bool ChangeWeapon(int32 _Idx);

	void CreateWeapons(ACharacter* _Owner);
	
	void Detach();
	
	bool AttachToBack();
	bool AttachToHand();

public:
	UWeaponManager();
	~UWeaponManager();

};
