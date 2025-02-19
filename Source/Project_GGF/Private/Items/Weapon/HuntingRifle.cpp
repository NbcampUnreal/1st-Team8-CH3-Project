#include "Items/Weapon/HuntingRifle.h"

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

	FVector MuzzleLocation = StaticMeshComp->GetComponentLocation();  // Static Mesh의 현재 위치
	FVector ForwardVector = StaticMeshComp->GetForwardVector();  // Mesh의 앞쪽 방향 벡터 (이 방향이 총구 방향)

	// 총구 끝 위치를 계산 (총구 끝 위치는 Mesh 크기의 절반을 벡터로 이동시킨 값)
	FVector MuzzleOffset = StaticMeshComp->GetStaticMesh()->GetBounds().BoxExtent; // 메시의 반지름
	MuzzleLocation += ForwardVector * MuzzleOffset.X;  // 앞쪽 끝으로 이동

	MuzzleLocation = MuzzleSceneComp->GetComponentLocation();

	// 총알 발사
	GetWorld()->SpawnActor<ABullet>(Bullet, MuzzleLocation, GetActorRotation());

	// 소음발생.

	// 탄약계산
	CurrentAmmo--;

	// 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AHuntingRifle::Reloading(int32 _TotalAmmo)
{
	return Super::Reloading(_TotalAmmo);
}
