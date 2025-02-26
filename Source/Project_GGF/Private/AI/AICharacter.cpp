#include "AICharacter.h"
#include "AIControllerCustom.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = AAIControllerCustom::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));

	HandSockets = { TEXT("L_HandSocket"), TEXT("R_HandSocket") };
	BackSockets = { TEXT("L_BackSocket"), TEXT("R_BackSocket") };
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponManager = Cast<UWeaponManager>(WeaponManagerPtr.GetDefaultObject());

	//if (WeaponManager)
		//WeaponManager->CreateWeapons(this);
}

void AAICharacter::Shoot()
{
	if (WeaponManager)
	{
		//WeaponManager->AttachToHand();
		WeaponManager->Attack();
	}
}

void AAICharacter::AddItemToInventory()
{
}
