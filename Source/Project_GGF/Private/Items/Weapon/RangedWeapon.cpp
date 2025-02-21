#include "Items/Weapon/RangedWeapon.h"
#include "TimerManager.h"  
#include "Items/Bullet/Bullet.h"
#include "Items/Bullet/TestBullet.h"
#include "Kismet/GameplayStatics.h"


ARangedWeapon::ARangedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	MuzzleSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSceneComp"));
	MuzzleSceneComp->SetupAttachment(StaticMeshComp);
}

ARangedWeapon::~ARangedWeapon()
{
}

void ARangedWeapon::PlaySound()
{
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleSceneComp->GetComponentLocation(), FireNoise);
	}
}

bool ARangedWeapon::Shot()
{
	return true;
}

bool ARangedWeapon::Reloading(int32 _NeededAmmo)
{
	if (_NeededAmmo <= 0)
		return 0;

	CurrentAmmo += _NeededAmmo;

	// 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &ARangedWeapon::EndReloading, ReloadingDelay, false);
	bIsReloading = true;

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
