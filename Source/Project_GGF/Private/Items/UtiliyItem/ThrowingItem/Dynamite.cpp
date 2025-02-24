#include "Items/UtiliyItem/ThrowingItem/Dynamite.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Project_GGF/Public/Character/Data/HealthComponent.h"

ADynamite::ADynamite()
{

}

void ADynamite::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActivationTimer, this, &ADynamite::Activation, Time, false);
}

void ADynamite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADynamite::Activation()
{
	Super::Activation();

}

void ADynamite::OnBulletOverlap(UPrimitiveComponent* _overlapComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (bIsStartActive)
	{
		if (_otherActor)
		{
			UHealthComponent* HealthComp = _otherActor->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				float StiffTime = 0.0f;

				// �±׿� ���� ���� �ð� �ٸ��� ����
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

				// ������ ����
				HealthComp->TakeDamage(this, EAttackType::Bullet, StiffTime, Damage);
			}
		}

		DestroyItem();
	}

}
