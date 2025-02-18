#include "Items/Weapon/HuntingRifle.h"

AHuntingRifle::AHuntingRifle()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponName = "HuntingRifle"; 
	FireDelay = 2.f;
	ReloadingDelay = 3.f;
	FireNoise = 3.f;
	Range = 300.f;
	Accuracy = 0.f;
	Recoil = 0.f;
	Damage = 100.f;
	MagazineCapacity = 15;
	CurrentAmmo = MagazineCapacity;
	bIsReloading = false;
	bIsFireDelay = false;



}

bool AHuntingRifle::Shot()
{
	return Super::Shot();
}

bool AHuntingRifle::Reloading(int32 _TotalAmmo)
{
	return Super::Reloading(_TotalAmmo);
}
