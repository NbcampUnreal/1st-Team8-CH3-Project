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

	FRotator MuzzleRotation = GetActorRotation();  // �ѱ� ����

	MuzzleLocation = MuzzleSceneComp->GetComponentLocation();

	float SpreadAngle = 10.0f;
	for (int32 i = 0; i < 5; i++)
	{
		float RandomYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
		float RandomPitch = FMath::RandRange(-SpreadAngle, SpreadAngle);

		// �� ���� ���
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
	// �����߻�.

	// ź����

	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ARangedWeapon::EndFireDelay, FireDelay, false);
	bIsFireDelay = true;
	return true;
}

bool AShotgun::Reloading(int32 _TotalAmmo)
{
	return false;
}


