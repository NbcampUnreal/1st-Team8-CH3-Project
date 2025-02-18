#include "Items/Weapon/RangedWeapon.h"
#include "TimerManager.h"  
#include "Items/Bullet/Bullet.h"


ARangedWeapon::ARangedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

//ARangedWeapon::ARangedWeapon(FString _Mesh)
//	: AWeapon(_Mesh)
//{
//	PrimaryActorTick.bCanEverTick = false;
//}

ARangedWeapon::~ARangedWeapon()
{
}

bool ARangedWeapon::Shot()
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
