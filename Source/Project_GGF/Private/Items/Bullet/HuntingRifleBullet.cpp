#include "Project_GGF/Public/Items/Bullet/HuntingRifleBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHuntingRifleBullet::AHuntingRifleBullet()
{
	//PrimaryActorTick.bCanEverTick = false;

	Range = 300.f;
	Accuracy = 0.f;
	Damage = 100.f;

	BulletType = EBulletType::HuntingRifle;

	ProjectileMovement->InitialSpeed = 15000;
	ProjectileMovement->MaxSpeed = 15000;
	ProjectileMovement->ProjectileGravityScale = 0.05f;

}

void AHuntingRifleBullet::BeginPlay()
{
	Super::BeginPlay();

}

void AHuntingRifleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

