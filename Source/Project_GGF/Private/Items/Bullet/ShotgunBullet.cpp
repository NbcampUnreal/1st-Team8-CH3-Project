#include "Items/Bullet/ShotgunBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

AShotgunBullet::AShotgunBullet()
{
	//PrimaryActorTick.bCanEverTick = false;

	Range = 100.f;
	Accuracy = 0.f;
	Damage = 100.f;

	BulletType = EBulletType::HuntingRifle;

	ProjectileMovement->InitialSpeed = 5000;
	ProjectileMovement->MaxSpeed = 5000;
	ProjectileMovement->ProjectileGravityScale = 0.05f;
}

void AShotgunBullet::BeginPlay()
{
	Super::BeginPlay();

}

void AShotgunBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

