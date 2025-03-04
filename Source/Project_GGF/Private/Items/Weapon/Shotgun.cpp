#include "Items/Weapon/Shotgun.h"
#include "Items/Bullet/Bullet.h"

AShotgun::AShotgun()
{

	WeaponName = "Shotgun";
	FireDelay = 2.f;
	ReloadingDelay = 3.f;
	FireNoise = 3.f;
	ShotAmmoCnt = 5;
	MagazineCapacity = ShotAmmoCnt * 2;
	Recoil = 0.f;
	SpreadAngle = 3.f;
	CurrentAmmo = MagazineCapacity;
	bIsReloading = false;
	bIsFireDelay = false;
	BulletType = EBulletType::Shotgun;
}

bool AShotgun::Shot()
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

	for (int32 i = 0; i < ShotAmmoCnt; i++)
	{
		float RandomYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
		float RandomPitch = FMath::RandRange(-SpreadAngle, SpreadAngle);
		
		FRotator SpreadRotation = MuzzleRotation + FRotator(RandomPitch, RandomYaw, 0);
		FVector ShotDirection = SpreadRotation.Vector();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(Bullet, MuzzleLocation, SpreadRotation, SpawnParams);

		if (bullet)
		{
			FVector Velocity = ShotDirection * bullet->GetProjectileInitialSpeed();
			bullet->SetProjectileVelocity(Velocity);
		}

		CurrentAmmo--;
	}
	
	PlaySound();
	PlayVFX();

	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AShotgun::Reloading(int32 _TotalAmmo)
{
	return false;
}


