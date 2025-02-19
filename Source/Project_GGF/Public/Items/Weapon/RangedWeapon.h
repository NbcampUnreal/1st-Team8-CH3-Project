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
	FName WeaponName;	// �����̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireDelay;	// �߻簣��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadingDelay;	// �������ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireNoise;	// ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Recoil;		// �ݵ� ũ��
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineCapacity;	// źâ�� �ִ�ź���
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo;		// ���� źȯ��
	
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
