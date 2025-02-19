#include "Project_GGF/Public/Items/Bullet/Bullet.h"
#include "Project_GGF/Public/Component/HealthComponent.h"
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
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

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
    if (_otherActor)
    {
        UHealthComponent* HealthComp = _otherActor->FindComponentByClass<UHealthComponent>();
        if (HealthComp)
        {
            float StiffTime = 0.0f;

            // 태그에 따라 경직 시간 다르게 적용
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

            // 데미지 적용
            HealthComp->TakeDamage(this, EAttackType::Bullet, StiffTime, Damage);
        }
    }

    BulletDestroy();
}

void ABullet::OnBulletEndOverlap(UPrimitiveComponent* _overlapComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		/*OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();*/
	}
}

void ABullet::OnProjectileStop(const FHitResult& _ImpacResult)
{
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ABullet::BulletDestroy, 2.f, false);
}

void ABullet::BulletDestroy()
{
	Destroy();
}

void ABullet::SetProjectileVelocity(FVector _Velocity)
{
	ProjectileMovement->Velocity = _Velocity;
}

float ABullet::GetProjectileInitialSpeed()
{
	return ProjectileMovement->InitialSpeed;
}