#include "Project_GGF/Public/Items/Weapon/Weapon.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);
}
void AWeapon::SpawnWeapon(FVector _Location, FRotator _Rotator)
{
	//()->SpawnActor(this, _Location, _Rotator);
}

void AWeapon::AttachWeapon(USceneComponent* _SceneComp)
{
	if (_SceneComp == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(_SceneComp, AttachmentRules, FName(TEXT("GripPoint")));

}
