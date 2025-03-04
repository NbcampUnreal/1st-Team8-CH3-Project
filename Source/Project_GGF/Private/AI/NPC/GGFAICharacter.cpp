#include "AI/NPC/GGFAICharacter.h"

AGGFAICharacter::AGGFAICharacter()
{
}

void AGGFAICharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponManager = Cast<UWeaponManager>(WeaponManagerPtr.GetDefaultObject());
	if (WeaponManager)
	{
		WeaponManager->CreateWeapons(this);
	}
}

void AGGFAICharacter::Shoot()
{
	if (WeaponManager)
	{
		WeaponManager->Attack();
	}
}
