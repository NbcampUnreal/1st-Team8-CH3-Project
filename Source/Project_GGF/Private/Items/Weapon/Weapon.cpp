#include "Items/Weapon/Weapon.h"

AWeapon::AWeapon()
	: SceneComp(nullptr)
	, StaticMeshComp(nullptr)
{
}

AWeapon::AWeapon(FString _Mesh)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);

	// Static Mesh�� �ڵ忡�� ����
	UStaticMesh* MeshAsset = LoadObject<UStaticMesh>(nullptr, *_Mesh);
	if (MeshAsset)
	{
		StaticMeshComp->SetStaticMesh(MeshAsset);
	}

	//// Material�� �ڵ忡�� ����
	//UMaterial* MaterialAsset = LoadObject<UMaterial>(nullptr, *_Material);
	//if (MaterialAsset)
	//{
	//	StaticMeshComp->SetMaterial(0, MaterialAsset);
	//}
}

