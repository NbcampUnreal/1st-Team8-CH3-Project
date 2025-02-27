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
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
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

void AThrowingItem::Activation()
{
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();

	bIsStartActive = true;
}

void AThrowingItem::OnBulletOverlap(UPrimitiveComponent* _overlapComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
}
