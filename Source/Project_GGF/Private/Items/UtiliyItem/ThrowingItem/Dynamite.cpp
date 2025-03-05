#include "Items/UtiliyItem/ThrowingItem/Dynamite.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Project_GGF/Public/Character/Data/HealthComponent.h"

ADynamite::ADynamite()
{
	ItemName = "Dynamite";

}

void ADynamite::BeginPlay()
{
	Super::BeginPlay();

}

void ADynamite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADynamite::Throw(FVector LaunchVelocity)
{
	Super::Throw(LaunchVelocity);

	GetWorld()->GetTimerManager().SetTimer(ActivationTimer, this, &ADynamite::Activation, Time, false);
}

void ADynamite::Activation()
{
	Super::Activation();

	TArray<AActor*> overlapActors;
	CollisionComp->GetOverlappingActors(overlapActors);

	for (AActor* actor : overlapActors)
	{
		if (actor)
		{
			UHealthComponent* HealthComp = actor->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				float StiffTime = 0.0f;

				if (actor->ActorHasTag("Player"))
				{
					StiffTime = 0.15f;
				}
				else if (actor->ActorHasTag("Enemy"))
				{
					StiffTime = 0.2f;
				}
				else if (actor->ActorHasTag("Creature"))
				{
					StiffTime = 0.5f;
				}

				HealthComp->TakeDamage(this, EAttackType::Bullet, StiffTime, Damage);
			}
		}
	}

	PlaySound();
	PlayVFX();

	DestroyItem();
}


