#include "Project_GGF/Public/Items/Bullet/Bullet.h"
#include "Project_GGF/Public/Character/Data/HealthComponent.h"
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
	ProjectileMovementComp->OnProjectileStop.AddDynamic(this, &ABullet::OnProjectileStop);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ABullet::OnBulletEndOverlap);

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
		ProjectileMovementComp->ProjectileGravityScale = 10.0f;
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* _overlapComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
    if (_otherActor)
    {
        UHealthComponent* HealthComp = _otherActor->FindComponentByClass<UHealthComponent>();
        if (HealthComp)
        {
            float StiffTime = 0.0f;

            // �±׿� ���� ���� �ð� �ٸ��� ����
            if (_otherActor->ActorHasTag("Player"))
            {
                StiffTime = 0.15f;
            }
            else if (_otherActor->ActorHasTag("Enemy"))
            {
                StiffTime = 0.2f;
            }
            else if (_otherActor->ActorHasTag("Creature"))
            {
                StiffTime = 0.5f;
            }

            // ������ ����
            HealthComp->TakeDamage(this, EAttackType::Bullet, StiffTime, Damage);
        }
    }

    BulletDestroy();
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
