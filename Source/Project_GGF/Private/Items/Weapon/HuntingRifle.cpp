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

	
	float RandomYaw = FMath::RandRange(-Recoil, Recoil);
	float RandomPitch = FMath::RandRange(-Recoil, Recoil);
	
	// 새 방향 계산
	FRotator SpreadRotation = MuzzleRotation + FRotator(RandomPitch, RandomYaw, 0);
	FVector ShotDirection = SpreadRotation.Vector();
	
	UWorld* World = GetWorld();
	if (!World)
		return false;
	

	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(Bullet, MuzzleLocation, SpreadRotation);
	
	if (bullet)
	{
		FVector Velocity = ShotDirection * bullet->GetProjectileInitialSpeed();
		bullet->SetProjectileVelocity(Velocity);
	}

	// 탄약계산
	CurrentAmmo--;

	// 소음발생.
	PlaySound();

	// 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AHuntingRifle::Reloading(int32 _TotalAmmo)
{
	return Super::Reloading(_TotalAmmo);
}
