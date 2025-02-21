#include "Project_GGF/Public/Items/Weapon/HuntingRifle.h"

AHuntingRifle::AHuntingRifle()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponName = "HuntingRifle"; 
	FireDelay = 2.f;
	ReloadingDelay = 3.f;
	FireNoise = 3.f;
	MagazineCapacity = 15;
	Recoil = 0.f;
	CurrentAmmo = MagazineCapacity;
	bIsReloading = false;
	bIsFireDelay = false;
	BulletType = EBulletType::HuntingRifle;
}

bool AHuntingRifle::Shot()
{
	return Super::Shot();
}

bool AHuntingRifle::Reloading(int32 _TotalAmmo)
{
	return Super::Reloading(_TotalAmmo);
}
