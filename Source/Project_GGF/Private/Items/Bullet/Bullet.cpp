#include "Items/Bullet/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"



ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	RootComponent = CollisionComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ABullet::OnProjectileStop);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ABullet::OnBulletEndOverlap);


	InitialLocation = GetActorLocation();
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();

	float DistanceTraveled = (CurrentLocation - InitialLocation).Size();

	if (DistanceTraveled > Range)
		ProjectileMovement->ProjectileGravityScale = 10.0f;
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* _overlapComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	int a = 0;
	
	if (_otherActor && _otherActor->ActorHasTag("Player"))
	{
	}
	else if (_otherActor && _otherActor->ActorHasTag("Enemy"))
	{
	}
	else if (_otherActor && _otherActor->ActorHasTag("Creature"))
	{
	}
}

void ABullet::OnBulletEndOverlap(UPrimitiveComponent* _overlapComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
}

void ABullet::OnProjectileStop(const FHitResult& _ImpacResult)
{
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ABullet::BulletDestroy, 2.f, false);
}

void ABullet::BulletDestroy()
{
	Destroy();
}
