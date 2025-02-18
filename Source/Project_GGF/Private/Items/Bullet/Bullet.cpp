#include "Items/Bullet/Bullet.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

