#include "Items/UtiliyItem/ThrowingItem/SmokeGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASmokeGrenade::ASmokeGrenade()
{

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


	ProjectileMovement->StopMovementImmediately();
}
