#include "Items/Bullet/HuntingRifleBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHuntingRifleBullet::AHuntingRifleBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	Range = 300.f;
	Accuracy = 0.f;
	Damage = 100.f;

	BulletType = EBulletType::HuntingRifle;

	ProjectileMovementComp->InitialSpeed = 3000;
	ProjectileMovementComp->MaxSpeed = 3000;
	ProjectileMovementComp->ProjectileGravityScale = 0.1f;

}

void AHuntingRifleBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHuntingRifleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

