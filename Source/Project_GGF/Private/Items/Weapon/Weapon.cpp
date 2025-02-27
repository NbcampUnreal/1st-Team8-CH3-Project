#include "Project_GGF/Public/Items/Weapon/Weapon.h"
#include "Components/SkeletalMeshComponent.h"


AWeapon::AWeapon()
	: bIsEquipped(false)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	
	// 생성 후 nullptr 체크
	if (StaticMeshComp)
	{
		SetRootComponent(StaticMeshComp);
	}
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
void AWeapon::Tick(float DeltaTime)
{
	/*if (bIsEquipped)
	{
		FVector RightHandLocation = GetActorLocation();
		FVector LeftHandLocation = LeftHandSceneComp->GetComponentLocation();

		FVector MidPoint = (RightHandLocation + LeftHandLocation) * 0.5f;

		FRotator NewRotation = (LeftHandLocation - RightHandLocation).Rotation();

		NewRotation.Pitch = 0.0f;
		NewRotation.Yaw = 0.0f;

		SetActorLocation(MidPoint);
		SetActorRotation(NewRotation);
	}*/
}
void AWeapon::SpawnWeapon(FVector _Location, FRotator _Rotator)
{
	//()->SpawnActor(this, _Location, _Rotator);
}


void AWeapon::AttachWeaponToBack(USkeletalMeshComponent* _SceneComp, FName _BoneName)
{
	if (_SceneComp == nullptr)
		return;

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(_SceneComp, AttachmentRules, _BoneName);

	bIsEquipped = false;
}

void AWeapon::AttachWeaponToHand(USkeletalMeshComponent* CharacterMesh, FName HandSocketName)
{
	if (CharacterMesh)
	{

		AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocketName);
		
	}
	bIsEquipped = true;
}
	
void AWeapon::AttachWeaponToSocket(USkeletalMeshComponent* CharacterMesh, FName HandSocketName, FName WeaponSocketName)
{
	if (CharacterMesh)
	{
		FTransform WeaponSocketTransform = StaticMeshComp->GetSocketTransform(WeaponSocketName);

		
		int32 BoneIndex = CharacterMesh->GetBoneIndex(HandSocketName);
		if (BoneIndex != INDEX_NONE)
		{
			CharacterMesh->BoneSpaceTransforms[BoneIndex].SetLocation(WeaponSocketTransform.GetLocation());
			CharacterMesh->BoneSpaceTransforms[BoneIndex].SetRotation(WeaponSocketTransform.GetRotation());
			
			CharacterMesh->RefreshBoneTransforms();
			CharacterMesh->UpdateComponentToWorld();
		}
	}
	bIsEquipped = true;
}

void AWeapon::HideWeapon()
{ 
	SetActorHiddenInGame(true);  
	SetActorEnableCollision(false);  
}

void AWeapon::ShowWeapon()
{
	SetActorHiddenInGame(false);  
	SetActorEnableCollision(true); 
}
