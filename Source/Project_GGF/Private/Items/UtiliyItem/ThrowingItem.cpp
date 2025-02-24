#include "Items/UtiliyItem/ThrowingItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Items/UtiliyItem/ThrowingItem/Dynamite.h"
#include "Items/UtiliyItem/ThrowingItem/SmokeGrenade.h"



AThrowingItem::AThrowingItem()
	: bIsStartActive(false)
{
	PrimaryActorTick.bCanEverTick = true;

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

	FThrowingItemDataTable* Data = ItemDataManager->GetThrowingItemDataTable(ItemName);
	Range = Data->Range;
	Time = Data->Time;
	Damage = Data->Damage;
	//InitialLifeSpan = 3.0f;
}

void AThrowingItem::BeginPlay()
{
	Super::BeginPlay();
}

void AThrowingItem::Tick(float DeltaTime)
{
}

void AThrowingItem::Activation()
{
	ProjectileMovement->StopMovementImmediately();

	bIsStartActive = true;
}

void AThrowingItem::OnBulletOverlap(UPrimitiveComponent* _overlapComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
}
