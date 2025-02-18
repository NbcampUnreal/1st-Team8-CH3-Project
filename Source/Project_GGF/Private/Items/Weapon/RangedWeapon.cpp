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
	// 현재 탄환개수가 0일경우
	if (CurrentAmmo == 0)
	{
		return false;
	}

	// 발사 딜레이
	if (bIsFireDelay)
	{
		return false;
	}

	if (bIsReloading)
	{
		return false;
	}
	
	// 총알 발사

	// 소음발생.

	// 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);

	return true;
}

bool ARangedWeapon::Reloading(int32 _TotalAmmo)
{

	// 타이머 설정
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
