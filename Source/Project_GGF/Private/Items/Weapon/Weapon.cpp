#include "Project_GGF/Public/Items/Weapon/Weapon.h"


AWeapon::AWeapon()
	: bIsEquipped(false)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftHandSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandSceneComp"));
	LeftHandSceneComp->SetupAttachment(SceneComp);
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
void AWeapon::Tick(float DeltaTime)
{
	if (bIsEquipped)
	{
		FVector RightHandLocation = GetActorLocation();
		FVector LeftHandLocation = LeftHandSceneComp->GetComponentLocation();

		FVector MidPoint = (RightHandLocation + LeftHandLocation) * 0.5f;

		FRotator NewRotation = (LeftHandLocation - RightHandLocation).Rotation();

		SetActorLocation(MidPoint);
		SetActorRotation(NewRotation);
	}
}
void AWeapon::SpawnWeapon(FVector _Location, FRotator _Rotator)
{
	//()->SpawnActor(this, _Location, _Rotator);
}

void AWeapon::AttachWeaponToBack(USceneComponent* _SceneComp)
{
	if (_SceneComp == nullptr)
		return;

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(_SceneComp, AttachmentRules, FName(TEXT("GripPoint")));
}

void AWeapon::AttachWeaponToHand(TArray<USceneComponent*> _SceneComp)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	LeftHandSceneComp->AttachToComponent(_SceneComp[0], AttachmentRules, FName(TEXT("GripPoint")));
	AttachToComponent(_SceneComp[1], AttachmentRules, FName(TEXT("GripPoint")));
}
