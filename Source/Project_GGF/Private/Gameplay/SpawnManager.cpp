﻿#include "Gameplay/SpawnManager.h"
#include "Landscape.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"


ASpawnManager::ASpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateSpawnPoints();
}

void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShowDebug)
	{
		for (const FVector& SpawnLocation : GridSpawnPoints)
		{
			//DrawDebugSphere(GetWorld(), SpawnLocation, 100.0f, 12, FColor::Red, true, -1.0f);
		}
	}
}

void ASpawnManager::GenerateSpawnPointsInEditor()
{
	GenerateSpawnPoints();
}

void ASpawnManager::GenerateSpawnPoints()
{
    GridSpawnPoints.Empty();

    FVector LandscapeSize = GetLandscapeSize();
    if (LandscapeSize.IsZero()) return;

    float StepX = LandscapeSize.X / (GridSize - 1);
    float StepY = LandscapeSize.Y / (GridSize - 1);

    for (int32 x = 1; x < GridSize - 1; x++)
    {
        for (int32 y = 1; y < GridSize - 1; y++)
        {
            FVector GridPoint(
				Origin.X - LandscapeSize.X / 2 + x * StepX,
				Origin.Y - LandscapeSize.Y / 2 + y * StepY,
                0.0f
            );

            GridPoint.Z = GetHeight(GridPoint) + 50.0f;

			if (GridPoint.Z > MaxSpawnHeight)
				continue;

			if (IsValidSpawnLocation(GridPoint))
			{
				GridSpawnPoints.Add(GridPoint);
			}
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Generated %d spawn points"), GridSpawnPoints.Num());
}


FVector ASpawnManager::GetLandscapeSize()
{
    Landscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
	if (!Landscape) return FVector::ZeroVector;

	Landscape->GetActorBounds(false, Origin, Extent);

	return Extent*2;
}


float ASpawnManager::GetHeight(const FVector& Location)
{
    if (!Landscape)
    {
        Landscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
        if (!Landscape) return 0.0f;
    }

    TOptional<float> HeightValue = Landscape->GetHeightAtLocation(Location);
    if (HeightValue.IsSet())
    {
        return HeightValue.GetValue();
    }

	FVector Center = Landscape->GetActorLocation();
	return Center.Z;
}


bool ASpawnManager::IsValidSpawnLocation(FVector Location)
{
	FVector BoxExtent(100.0f, 100.0f, 50.0f); //AI 크기 나중에 체크하기
	FBox SpawnBox(Location - BoxExtent, Location + BoxExtent);

	TArray<AActor*> CollisionActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), CollisionActors);

	for (AActor* Actor : CollisionActors)
	{
		FVector ActorOrigin, ActorExtent;
		Actor->GetActorBounds(false, ActorOrigin, ActorExtent);
		FBox ActorBox(ActorOrigin - ActorExtent, ActorOrigin + ActorExtent);

		UE_LOG(LogTemp, Warning, TEXT("Checking Collision: Actor Origin (%f, %f, %f)"),
			ActorOrigin.X, ActorOrigin.Y, ActorOrigin.Z);

		// 3D 불린 연산
		if (SpawnBox.Intersect(ActorBox))
		{
			return false;
		}
	}

	return true;
}

