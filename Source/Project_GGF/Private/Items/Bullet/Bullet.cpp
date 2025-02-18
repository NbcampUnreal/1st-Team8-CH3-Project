#include "Items/Bullet/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoventComp"));
	ProjectileMovementComp->InitialSpeed = 1500;
	ProjectileMovementComp->MaxSpeed = 10000;
	ProjectileMovementComp->ProjectileGravityScale = 0.5f;

}

ABullet::ABullet(FString _Mesh)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);

	// Static Mesh를 코드에서 설정
	UStaticMesh* MeshAsset = LoadObject<UStaticMesh>(nullptr, *_Mesh);
	if (MeshAsset)
	{
		StaticMeshComp->SetStaticMesh(MeshAsset);
	}

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoventComp"));
	ProjectileMovementComp->InitialSpeed = 1500;
	ProjectileMovementComp->MaxSpeed = 10000;
	ProjectileMovementComp->ProjectileGravityScale = 0.5f;

}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

