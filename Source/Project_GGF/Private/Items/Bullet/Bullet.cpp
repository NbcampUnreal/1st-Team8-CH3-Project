#include "Items/Bullet/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"



ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SetRootComponent(StaticMeshComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleComp"));
	SphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComp->SetupAttachment(StaticMeshComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoventComp"));

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ABullet::OnBulletEndOverlap);

}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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