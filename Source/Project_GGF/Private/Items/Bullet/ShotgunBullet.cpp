#include "Project_GGF/Public/Items/Bullet/ShotgunBullet.h"

AShotgunBullet::AShotgunBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AShotgunBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShotgunBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

