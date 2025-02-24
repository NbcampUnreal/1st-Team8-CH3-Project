#include "Items/UtiliyItem/ThrowingItem/SmokeGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASmokeGrenade::ASmokeGrenade()
	: CurrentScale(1.0f)
	, MaxScale(10.0f)
	, GrowSpeed(0.1f)
{

	ProjectileMovement->InitialSpeed = 1500.0f;
	ProjectileMovement->MaxSpeed = 1500.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 1.0f; 

	SmokeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeParticle"));
	SmokeParticle->SetupAttachment(RootComponent);
	SmokeParticle->SetAutoActivate(false);

}

void ASmokeGrenade::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActivationTimer, this, &ASmokeGrenade::Activation, Time, false);
}

void ASmokeGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ASmokeGrenade::Activation()
{
	Super::Activation();

	if (SmokeEffect)
	{
		SmokeParticle->SetTemplate(SmokeEffect);
		SmokeParticle->Activate();
	}
	GetWorldTimerManager().SetTimer(GrowTimer, this, &ASmokeGrenade::GrowSmoke, 0.1f, true);

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ASmokeGrenade::DestroySmoke, Duration, false);
}

void ASmokeGrenade::GrowSmoke()
{
	if (CurrentScale < MaxScale)
	{
		CurrentScale += GrowSpeed;
		SmokeParticle->SetWorldScale3D(FVector(CurrentScale));
	}
	else
	{
		GetWorldTimerManager().ClearTimer(GrowTimer);
	}
}

void ASmokeGrenade::DestroySmoke()
{
	DestroyItem();
}