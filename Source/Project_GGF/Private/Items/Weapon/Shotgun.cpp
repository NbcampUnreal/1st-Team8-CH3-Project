#include "Items/Weapon/Shotgun.h"
#include "Items/Bullet/Bullet.h"

AShotgun::AShotgun()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponName = "Shotgun";
	FireDelay = 2.f;
	ReloadingDelay = 3.f;
	FireNoise = 3.f;
	ShotAmmoCnt = 5;
	MagazineCapacity = ShotAmmoCnt*2;
	Recoil = 0.f;
	SpreadAngle = 3.f;
	CurrentAmmo = MagazineCapacity;
	bIsReloading = false;
	bIsFireDelay = false;
	BulletType = EBulletType::Shotgun;
}

bool AShotgun::Shot()
{
	// 현재 탄환개수가 0일경우
	if (CurrentAmmo <= 0)
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

	FVector MuzzleLocation = MuzzleSceneComp->GetComponentLocation();
	FRotator MuzzleRotation = GetActorRotation();  // 총구 방향

	for (int32 i = 0; i < ShotAmmoCnt; i++)
	{
		float RandomYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
		float RandomPitch = FMath::RandRange(-SpreadAngle, SpreadAngle);

		// 새 방향 계산
		FRotator SpreadRotation = MuzzleRotation + FRotator(RandomPitch, RandomYaw, 0);
		FVector ShotDirection = SpreadRotation.Vector();

		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(Bullet, MuzzleLocation, SpreadRotation);

		if (bullet)
		{
			FVector Velocity = ShotDirection * bullet->GetProjectileInitialSpeed();
			bullet->SetProjectileVelocity(Velocity);
		}

		CurrentAmmo--;
	}

	// 소음발생.
	PlaySound();

	// 탄약계산

	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AShotgun::Reloading(int32 _TotalAmmo)
{
	return false;
}


