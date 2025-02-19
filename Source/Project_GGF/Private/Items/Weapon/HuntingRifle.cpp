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
	// ���� źȯ������ 0�ϰ��
	if (CurrentAmmo <= 0)
	{
		return false;
	}

	// �߻� ������
	if (bIsFireDelay)
	{
		return false;
	}

	if (bIsReloading)
	{
		return false;
	}

	FVector MuzzleLocation = StaticMeshComp->GetComponentLocation();  // Static Mesh�� ���� ��ġ
	FVector ForwardVector = StaticMeshComp->GetForwardVector();  // Mesh�� ���� ���� ���� (�� ������ �ѱ� ����)

	// �ѱ� �� ��ġ�� ��� (�ѱ� �� ��ġ�� Mesh ũ���� ������ ���ͷ� �̵���Ų ��)
	FVector MuzzleOffset = StaticMeshComp->GetStaticMesh()->GetBounds().BoxExtent; // �޽��� ������
	MuzzleLocation += ForwardVector * MuzzleOffset.X;  // ���� ������ �̵�

	MuzzleLocation = MuzzleSceneComp->GetComponentLocation();

	// �Ѿ� �߻�
	GetWorld()->SpawnActor<ABullet>(Bullet, MuzzleLocation, GetActorRotation());

	// �����߻�.

	// ź����
	CurrentAmmo--;

	// Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AHuntingRifle::Reloading(int32 _TotalAmmo)
{
	return Super::Reloading(_TotalAmmo);
}
