#include "Character/Data/HitDeadComponent.h"
#include "Character/GGFCharacterBase.h"

void UHitDeadComponent::PlayHitMontage()
{
	AGGFCharacterBase* Character = Cast<AGGFCharacterBase>(GetOwner());
	if (Character == nullptr)
		return;

	Character->PlayAnimMontage(DeadMontage);
}

void UHitDeadComponent::PlayDeadMontage()
{
	AGGFCharacterBase* Character = Cast<AGGFCharacterBase>(GetOwner());
	if (Character == nullptr)
		return;

	Character->PlayAnimMontage(DeadMontage);
}
