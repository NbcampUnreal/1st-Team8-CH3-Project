#include "AICharacter.h"
#include "AIControllerCustom.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = AAIControllerCustom::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	WeaponSocket = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSocket"));
	WeaponSocket->SetupAttachment(GetMesh(), FName("hand_r"));
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponManager = Cast<UWeaponManager>(WeaponManagerPtr.GetDefaultObject());

	if (WeaponManager)
		WeaponManager->CreateWeapons(this);
}

void AAICharacter::Shoot()
{
	if (WeaponManager)
	{
		WeaponManager->Attack();
	}

	if (FireMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(FireMontage);
	}
}
