#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_GGF/Public/Items/Weapon/Weapon.h"
#include "Project_GGF/Public/Items/Bullet/Bullet.h"
#include "RangedWeapon.generated.h"


UCLASS()
class PROJECT_GGF_API ARangedWeapon : public AWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABullet> Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponName;	// 무기이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireDelay;	// 발사간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadingDelay;	// 재장전속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireNoise;	// 소음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Recoil;		// 반동 크기
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineCapacity;	// 탄창당 최대탄약수
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo;		// 현재 탄환수
	
	UPROPERTY(EditAnywhere)
	EBulletType BulletType;

	bool bIsFireDelay;
	bool bIsReloading;

	FTimerHandle ReloadingTimer;
	FTimerHandle DelayTimer;


public:
	UFUNCTION(BlueprintCallable)
	virtual bool Shot();

	UFUNCTION(BlueprintCallable)
	virtual bool Reloading(int32 _NeededAmmo);

	void EndFireDelay();
	void EndReloading();
	
	FName GetName() { return WeaponName; } 
	
	bool GetIsReloading() { return bIsReloading; }

	EBulletType GetBulletType() { return BulletType ; }

	int32 GetMagazineCapacity() { return MagazineCapacity; }
	int32 GetCurrentAmmo() { return CurrentAmmo; }

public:	
	ARangedWeapon();
	//ARangedWeapon(FString _Mesh);
	~ARangedWeapon();
};
