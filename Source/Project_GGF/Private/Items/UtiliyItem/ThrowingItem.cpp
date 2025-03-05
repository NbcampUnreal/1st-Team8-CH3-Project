#include "Items/UtiliyItem/ThrowingItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"




AThrowingItem::AThrowingItem()
	: bIsStartActive(false)
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	SetRootComponent(CollisionComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.5f;
	ProjectileMovement->Friction = 0.2f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
}

void AThrowingItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemDataManagerClass != nullptr)
	{
		ItemDataManager = ItemDataManagerClass.GetDefaultObject();
	}

	if (ItemDataManager != nullptr)
	{
		FThrowingItemDataTable* Data = ItemDataManager->GetThrowingItemDataTable(ItemName);
		Range = Data->Range;
		Time = Data->Time;
		Damage = Data->Damage;
		Duration = Data->Duration;

	}
}

void AThrowingItem::Tick(float DeltaTime)
{
}

void AThrowingItem::Throw(FVector LaunchVelocity)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = LaunchVelocity;
	}
}

void AThrowingItem::Activation()
{
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();

	bIsStartActive = true;
}
