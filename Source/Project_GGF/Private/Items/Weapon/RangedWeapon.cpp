#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "TimerManager.h"  
#include "Project_GGF/Public/Items/Bullet/Bullet.h"


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

	// ���� źȯ������ 0�ϰ��
	if (CurrentAmmo <= 0 || bIsFireDelay || bIsReloading)
	{
		return false;
	}
	
	FVector MuzzleLocation = StaticMeshComp->GetComponentLocation();  // Static Mesh�� ���� ��ġ
	FVector ForwardVector = StaticMeshComp->GetForwardVector();  // Mesh�� ���� ���� ���� (�� ������ �ѱ� ����)

	// �ѱ� �� ��ġ�� ��� (�ѱ� �� ��ġ�� Mesh ũ���� ������ ���ͷ� �̵���Ų ��)
	FVector MuzzleOffset = StaticMeshComp->GetStaticMesh()->GetBounds().BoxExtent; // �޽��� ������
	MuzzleLocation += ForwardVector * MuzzleOffset.X;  // ���� ������ �̵�

	// �Ѿ� �߻�
	GetWorld()->SpawnActor<ABullet>(Bullet, MuzzleLocation, GetActorRotation());

	// �����߻�.

	// ź����
	CurrentAmmo--;

	bIsFireDelay = true;

	if (GetWorld()->GetTimerManager().IsTimerActive(DelayTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
	}

	// Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	return true;
}

bool ARangedWeapon::Reloading(int32 _NeededAmmo)
{
	if (_NeededAmmo <= 0)
		return 0;

	CurrentAmmo += _NeededAmmo;

	// Ÿ�̸� ����
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
