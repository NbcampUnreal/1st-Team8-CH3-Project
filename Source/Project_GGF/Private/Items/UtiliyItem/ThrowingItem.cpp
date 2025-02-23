#include "Items/UtiliyItem/ThrowingItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"



AThrowingItem::AThrowingItem()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	//CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetupAttachment(SceneComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;


	//InitialLifeSpan = 3.0f;
}

void AThrowingItem::BeginPlay()
{
	Super::BeginPlay();

	// 자식의 Begin에서 돌리기.
	//GetWorld()->GetTimerManager().SetTimer(ActivationTimer, this, &AThrowingItem::Activation, Time, false);

}

void AThrowingItem::Tick(float DeltaTime)
{
}

void AThrowingItem::Activation()
{
}