#include "Items/Weapon/RangedWeapon.h"
#include "TimerManager.h"  
#include "Items/Bullet/Bullet.h"
#include "Items/Bullet/TestBullet.h"
#include "Kismet/GameplayStatics.h"


ARangedWeapon::ARangedWeapon()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftHandSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandSceneComp"));
	LeftHandSceneComp->SetupAttachment(SceneComp);

	MuzzleSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSceneComp"));
	MuzzleSceneComp->SetupAttachment(SceneComp);

	WeaponSockets = { TEXT("Rifle_L_Socket"), TEXT("Rifle_R_Socket") };
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
