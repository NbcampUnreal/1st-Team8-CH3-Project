#include "Items/Bullet/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;



	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SetRootComponent(StaticMeshComp);

	CapsuleComp = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CapsuleComp->SetupAttachment(StaticMeshComp);

	// Static Mesh를 코드에서 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	// Material을 코드에서 설정
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}

	SetActorScale3D(FVector(0.3f, 0.1f, 0.1f));

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoventComp"));
	ProjectileMovementComp->InitialSpeed = 5000;
	ProjectileMovementComp->MaxSpeed = 10000;
	ProjectileMovementComp->ProjectileGravityScale = 0.1f;

}

ABullet::ABullet(FString _Mesh)
{
	//PrimaryActorTick.bCanEverTick = true;

	//CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	//CapsuleComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//SetRootComponent(CapsuleComp);

	//StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	//StaticMeshComp->SetupAttachment(CapsuleComp);

	//// Static Mesh를 코드에서 설정
	//UStaticMesh* MeshAsset = LoadObject<UStaticMesh>(nullptr, *_Mesh);
	//if (MeshAsset)
	//{
	//	StaticMeshComp->SetStaticMesh(MeshAsset);
	//}

	//ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoventComp"));
	//ProjectileMovementComp->InitialSpeed = 1500;
	//ProjectileMovementComp->MaxSpeed = 10000;
	//ProjectileMovementComp->ProjectileGravityScale = 0.5f;

	
}

ABullet::ABullet(FString _Mesh, FString _Material)
{
	//PrimaryActorTick.bCanEverTick = true;

	//CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	//CapsuleComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//SetRootComponent(CapsuleComp);

	//StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	//StaticMeshComp->SetupAttachment(CapsuleComp);

	//// Static Mesh를 코드에서 설정
	//UStaticMesh* MeshAsset = LoadObject<UStaticMesh>(nullptr, *_Mesh);
	//if (MeshAsset)
	//{
	//	StaticMeshComp->SetStaticMesh(MeshAsset);
	//}

	//// Material을 코드에서 설정
	//UMaterial* MaterialAsset = LoadObject<UMaterial>(nullptr, *_Material);
	//if (MaterialAsset)
	//{
	//	StaticMeshComp->SetMaterial(0, MaterialAsset);
	//}

	//ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoventComp"));
	//ProjectileMovementComp->InitialSpeed = 1500;
	//ProjectileMovementComp->MaxSpeed = 10000;
	//ProjectileMovementComp->ProjectileGravityScale = 0.5f;

	
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

