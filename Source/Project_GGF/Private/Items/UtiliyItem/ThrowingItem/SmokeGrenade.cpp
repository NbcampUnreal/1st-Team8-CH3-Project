#include "Items/UtiliyItem/ThrowingItem/SmokeGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASmokeGrenade::ASmokeGrenade()
	: CurrentScale(1.0f)
	, MaxScale(5.0f)
	, GrowSpeed(0.1f)
{
	ItemName = "SmokeGrenade";

	SmokeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeParticle"));
	SmokeParticle->SetupAttachment(RootComponent);
	SmokeParticle->SetAutoActivate(false);

}

void ASmokeGrenade::BeginPlay()
{
	Super::BeginPlay();

}

void ASmokeGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmokeGrenade::Activation()
{
	Super::Activation();

	//SetActorHiddenInGame(true);
	//SmokeParticle->SetVisibility(true);

	if (SmokeEffect)
	{
		SmokeParticle->SetTemplate(SmokeEffect);
		SmokeParticle->Activate();
	}
	GetWorldTimerManager().SetTimer(GrowTimer, this, &ASmokeGrenade::GrowSmoke, GrowSpeed, true);

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ASmokeGrenade::DestroySmoke, Duration, false);
}

void ASmokeGrenade::Throw(FVector LaunchVelocity)
{
	Super::Throw(LaunchVelocity);

	GetWorldTimerManager().SetTimer(ActivationTimer, this, &ASmokeGrenade::Activation, Time, false);
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