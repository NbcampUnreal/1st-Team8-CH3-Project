#include "Items/Weapon/Shotgun.h"
#include "Items/Bullet/Bullet.h"

AShotgun::AShotgun()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponName = "Shotgun";
	FireDelay = 2.f;
	ReloadingDelay = 3.f;
	FireNoise = 3.f;
	MagazineCapacity = 15;
	Recoil = 0.f;
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

	FVector MuzzleLocation = StaticMeshComp->GetComponentLocation();  // Static Mesh의 현재 위치
	FVector ForwardVector = StaticMeshComp->GetForwardVector();  // Mesh의 앞쪽 방향 벡터 (이 방향이 총구 방향)

	// 총구 끝 위치를 계산 (총구 끝 위치는 Mesh 크기의 절반을 벡터로 이동시킨 값)
	FVector MuzzleOffset = StaticMeshComp->GetStaticMesh()->GetBounds().BoxExtent; // 메시의 반지름
	MuzzleLocation += ForwardVector * MuzzleOffset.X;  // 앞쪽 끝으로 이동

	FRotator MuzzleRotation = GetActorRotation();  // 총구 방향

	MuzzleLocation = MuzzleSceneComp->GetComponentLocation();

	float SpreadAngle = 10.0f;
	for (int32 i = 0; i < 5; i++)
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

	// 탄약계산

	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AShotgun::Reloading(int32 _TotalAmmo)
{
	return false;
}


