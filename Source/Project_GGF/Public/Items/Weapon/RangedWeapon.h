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
	FName WeaponName;	// �����̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireDelay;	// �߻簣��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadingDelay;	// �������ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireNoise;	// ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;		// ��Ÿ�
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Accuracy;		// ��Ȯ��
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Recoil;		// �ݵ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Damage;		// ���ݷ�
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineCapacity;	// źâ�� �ִ�ź���
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo;		// ���� źȯ��
	
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
