#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Weapon/Weapon.h"
#include "RangedWeapon.generated.h"

UCLASS()
class PROJECT_GGF_API ARangedWeapon : public AWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponName;	// 무기이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireDelay;	// 발사간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadingDelay;	// 재장전속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireNoise;	// 소음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;		// 사거리
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Accuracy;		// 정확도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Recoil;		// 반동 크기

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Damage;		// 공격력
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineCapacity;	// 탄창당 최대탄약수
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo;		// 현재 탄환수
	
	bool bIsFireDelay;
	bool bIsReloading;

	FTimerHandle ReloadingTimer;
	FTimerHandle DelayTimer;

public:
	virtual bool Shot();
	virtual bool Reloading(int32 _TotalAmmo);

	void EndFireDelay();
	void EndReloading();
	
	FName GetName() { return WeaponName; } 
	bool GetIsReloading() { return bIsReloading; }

public:	
	ARangedWeapon();
};
