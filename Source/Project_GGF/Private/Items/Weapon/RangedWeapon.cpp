#include "Items/Weapon/RangedWeapon.h"
#include "TimerManager.h"  


ARangedWeapon::ARangedWeapon()
{
}

ARangedWeapon::ARangedWeapon(FString _Mesh)
	: AWeapon(_Mesh)
{
	PrimaryActorTick.bCanEverTick = false;
}

ARangedWeapon::~ARangedWeapon()
{
}

bool ARangedWeapon::Shot()
{
	// ���� źȯ������ 0�ϰ��
	if (CurrentAmmo == 0)
	{
		return false;
	}

	// �߻� ������
	if (bIsFireDelay)
	{
		return false;
	}

	if (bIsReloading)
	{
		return false;
	}
	
	// �Ѿ� �߻�

	// �����߻�.

	// Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);

	return true;
}

bool ARangedWeapon::Reloading(int32 _TotalAmmo)
{

	// Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &ARangedWeapon::EndReloading, ReloadingDelay, false);

	return true;
}

void ARangedWeapon::EndFireDelay()
{
	bIsFireDelay = false;
}

void ARangedWeapon::EndReloading()
{
	bIsReloading = false;
}
