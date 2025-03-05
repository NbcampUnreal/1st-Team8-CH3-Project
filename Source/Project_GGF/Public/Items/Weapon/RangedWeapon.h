#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

	UPROPERTY(VisibleAnywhere)
	USceneComponent* MuzzleSceneComp;

	// TODO: 이펙트 관련 컴포넌트 구성----------------------------------------------
	// 이펙트 컴포넌트------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Component")
	UNiagaraComponent* MuzzleNiagaraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Component")
	UNiagaraSystem* MuzzleNiagaraSys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Component")
	UAudioComponent* MuzzleAudioComp;
	
	// ----------------------------------------------------------------------------

	//FIXME: 파이어 사운드는 불필요해보일 것으로 보임. 오디오 컴포넌트로 대체 가능
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Sound")
	// USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	FName WeaponName;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	float FireDelay;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	float ReloadingDelay;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	float FireNoise;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	float Recoil;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	int32 CurrentAmmo;		

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName Weapon_Left = "Rifle_L_Socket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName Weapon_Right = "Rifle_R_Socket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<FName> WeaponSockets;

	
	UPROPERTY(EditAnywhere)
	EBulletType BulletType;

	bool bIsFireDelay;
	bool bIsReloading;

	FTimerHandle ReloadingTimer;
	FTimerHandle DelayTimer;

protected:
	virtual void PlaySound();

	virtual void PlayVFX();

public:
	UFUNCTION(BlueprintCallable)
	virtual bool Shot(FVector AimPoint);

	UFUNCTION(BlueprintCallable)
	virtual bool Reloading(int32 _NeededAmmo);

	void EndFireDelay();
	void EndReloading();

	FName GetName() { return WeaponName; }

	bool GetIsReloading() { return bIsReloading; }

	EBulletType GetBulletType() { return BulletType; }

	int32 GetMagazineCapacity() { return MagazineCapacity; }
	int32 GetCurrentAmmo() { return CurrentAmmo; }
	
	
	FName GetLSockets() const { return Weapon_Left; }
	FName GetRSockets() const { return Weapon_Right; }

public:
	ARangedWeapon();
	//ARangedWeapon(FString _Mesh);
	~ARangedWeapon();
};
