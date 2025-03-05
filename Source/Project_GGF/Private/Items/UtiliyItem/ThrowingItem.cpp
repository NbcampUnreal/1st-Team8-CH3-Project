#include "Items/UtiliyItem/ThrowingItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraActor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"



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

	// FX
	// 나이아가라 컴포넌트 생성 및 설정
	MuzzleNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleNiagaraComp"));
	MuzzleNiagaraComp->SetupAttachment(CollisionComp);
	MuzzleNiagaraComp->SetAutoActivate(false); 

	// 오디오 컴포넌트 생성 및 설정
	MuzzleAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MuzzleAudioComp"));
	MuzzleAudioComp->SetupAttachment(CollisionComp);
	MuzzleAudioComp->SetAutoActivate(false); 
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

void AThrowingItem::PlaySound()
{
	if (MuzzleAudioComp)
	{
		MuzzleAudioComp->Activate(true);
	}
}

void AThrowingItem::PlayVFX()
{
	if (MuzzleNiagaraComp)
	{
		MuzzleNiagaraComp->OnSystemFinished.AddDynamic(this, &AThrowingItem::OnNiagaraSystemFinished);
		MuzzleNiagaraComp->Activate(true);
	}
}

void AThrowingItem::PlayCameraShake()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (FireCameraShakeClass)
			PC->ClientStartCameraShake(FireCameraShakeClass);
	}
}

void AThrowingItem::OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent)
{
	Destroy();
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
