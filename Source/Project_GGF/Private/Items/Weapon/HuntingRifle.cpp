#include "Project_GGF/Public/Items/Weapon/HuntingRifle.h"

AHuntingRifle::AHuntingRifle()
{

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

bool AHuntingRifle::Shot(FVector AimPoint)
{
	if (CurrentAmmo <= 0)
	{
		return false;
	}

	if (bIsFireDelay)
	{
		return false;
	}

	if (bIsReloading)
	{
		return false;
	}

	FVector MuzzleLocation = MuzzleSceneComp->GetComponentLocation();
	FRotator MuzzleRotation = MuzzleSceneComp->GetComponentRotation();


	float RandomYaw = FMath::RandRange(-Recoil, Recoil);
	float RandomPitch = FMath::RandRange(-Recoil, Recoil);

	FRotator SpreadRotation = MuzzleRotation + FRotator(RandomPitch, RandomYaw, 0);
	FVector ShotDirection = SpreadRotation.Vector();

	FVector MuzzleToAimDirection = (AimPoint - MuzzleLocation).GetSafeNormal();
	FTransform BulletSpawnTransform(FRotator::ZeroRotator, MuzzleLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(Bullet, BulletSpawnTransform, SpawnParams);

	//ABullet* bullet = GetWorld()->SpawnActor<ABullet>(Bullet, MuzzleLocation, SpreadRotation);

	if (bullet)
	{
		//FVector Velocity = ShotDirection * bullet->GetProjectileInitialSpeed();
		FVector Velocity = MuzzleToAimDirection * bullet->GetProjectileInitialSpeed();
		bullet->SetProjectileVelocity(Velocity);
	}

	CurrentAmmo--;

	PlaySound();
	PlayVFX();
	PlayCameraShake();
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AHuntingRifle::Reloading(int32 _TotalAmmo)
{
	return Super::Reloading(_TotalAmmo);
}
