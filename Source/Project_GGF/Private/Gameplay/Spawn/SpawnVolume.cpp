#include "Gameplay/Spawn/SpawnVolume.h"
#include "AI/GGFAICharacterBase.h"
#include "Components/BoxComponent.h"
#include "Gameplay/Spawn/GroupManager.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnVolume::SpawnGroup()
{
	// 그룹 매니저
	AGroupManager* GroupMgr = GetWorld()->SpawnActor<AGroupManager>(AGroupManager::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	// 그룹원 스폰
	for (int32 i = 0; i < GroupSize; i++)
	{
		FVector SpawnLoc = GetRandomPointInVolume();
		SpawnLoc.Z += 100.0f;
		
		AGGFAICharacterBase* Spawned = GetWorld()->SpawnActor<AGGFAICharacterBase>(GroupSpawnClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);
		if (Spawned && GroupMgr)
		{
			GroupMgr->RegisterMember(Spawned);
		}
	}
	
}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	if (!SpawnBox) return GetActorLocation();

	FVector Extent = SpawnBox->GetScaledBoxExtent();
	FVector Origin = SpawnBox->GetComponentLocation();

	// 박스 내부 랜덤 좌표
	FVector RandPos = FVector(
		FMath::RandRange(-Extent.X, Extent.X),
		FMath::RandRange(-Extent.Y, Extent.Y),
		FMath::RandRange(-Extent.Z, Extent.Z)
	);

	return Origin + RandPos;
}
