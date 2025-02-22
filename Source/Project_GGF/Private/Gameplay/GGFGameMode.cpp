#include "Gameplay/GGFGameMode.h"
#include "Gameplay/GGFGameState.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Landscape.h"
#include "LandscapeComponent.h"
#include "Engine/StaticMeshActor.h"

AGGFGameMode::AGGFGameMode()
{
	GameStateClass = AGGFGameState::StaticClass();
}

void AGGFGameMode::BeginPlay()
{
    Super::BeginPlay();

    InitializeSpawnPoints();

    SpawnAI(ESpawnType::Bear, BearCount);
    SpawnAI(ESpawnType::Boar, BoarCount);
    SpawnAI(ESpawnType::DeerDoe, DeerDoeCount);
    SpawnAI(ESpawnType::DeerStag, DeerStagCount);
    SpawnAI(ESpawnType::AICharacter, AICharacterCount);

    GetWorldTimerManager().SetTimer(GameTimeHandle, this, &AGGFGameMode::UpdateGameTime, 1.0f, true);
}

void AGGFGameMode::UpdateGameTime()
{
    CurrentTime += 1.0f;

    if (CurrentTime >= TotalGameTime)
    {
        //UE_LOG(LogTemp, Error, TEXT("베드 엔딩"));
        // 게임 종료 구현하기
    }
}

float AGGFGameMode::GetGameTimeRatio() const
{
    return FMath::Clamp(CurrentTime / TotalGameTime, 0.0f, 1.0f);
}

void AGGFGameMode::InitializeSpawnPoints()
{
    LandscapeHeightData();
    GenerateGridPoints(20);
}

void AGGFGameMode::LandscapeHeightData()
{
    HeightMap.Empty();

    ALandscape* Landscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
    if (!Landscape) return;

    for (UActorComponent* Component : Landscape->GetComponents())
    {
        ULandscapeComponent* LandscapeComponent = Cast<ULandscapeComponent>(Component);
        if (!LandscapeComponent) continue;

        FVector ComponentLocation = LandscapeComponent->GetComponentLocation();
        HeightMap.Add(FIntPoint(FMath::RoundToInt(ComponentLocation.X), FMath::RoundToInt(ComponentLocation.Y)), ComponentLocation.Z);
    }
}

void AGGFGameMode::GenerateGridPoints(int32 GridSize)
{
    GridSpawnPoints.Empty();

    FVector LandscapeSize = GetLandscapeSize();
    if (LandscapeSize.IsZero()) return;

    float StepX = LandscapeSize.X / GridSize;
    float StepY = LandscapeSize.Y / GridSize;

    for (int32 x = 0; x < GridSize; x++)
    {
        for (int32 y = 0; y < GridSize; y++)
        {
            FVector GridPoint(
                x * StepX,
                y * StepY,
                0.0f
            );

            GridPoint.Z = GetHeight(GridPoint);
            GridSpawnPoints.Add(GridPoint);
        }
    }
}


FVector AGGFGameMode::GetLandscapeSize()
{
    ALandscape* Landscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));

    if (!Landscape) return FVector::ZeroVector;

    FVector Origin, Extent;
    Landscape->GetActorBounds(false, Origin, Extent);

    return Extent * 2;
}


float AGGFGameMode::GetHeight(FVector WorldPosition)
{
    FIntPoint GridKey(FMath::RoundToInt(WorldPosition.X), FMath::RoundToInt(WorldPosition.Y));

    if (HeightMap.Contains(GridKey))
    {
        return HeightMap[GridKey];
    }

    return 0.0f;
}


void AGGFGameMode::SpawnAI(ESpawnType SpawnType, int32 Count)
{
	if (!AIClasses.Contains(SpawnType)) return;

	TSubclassOf<ACharacter> AIClass = AIClasses[SpawnType];
	if (!AIClass) return;

    int32 SpawnedCount = 0;

    while (SpawnedCount < Count && GridSpawnPoints.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, GridSpawnPoints.Num() - 1);
        FVector SpawnLocation = GridSpawnPoints[Index];

        if (!IsValidSpawnLocation(SpawnLocation))
        {
            GridSpawnPoints.RemoveAt(Index);
            continue;
        }

        // 높이 보정 
        SpawnLocation.Z += 50.0f;

        // AI 스폰
        ACharacter* SpawnedAI = GetWorld()->SpawnActor<ACharacter>(AIClass, SpawnLocation, FRotator::ZeroRotator);
        if (SpawnedAI)
        {
            SpawnedCount++;
        }

        // 사용한 위치 제거 
        GridSpawnPoints.RemoveAt(Index);
    }
}

bool AGGFGameMode::IsValidSpawnLocation(FVector Location)
{
    FVector BoxExtent(100.0f, 100.0f, 50.0f);
    FBox SpawnBox(Location - BoxExtent, Location + BoxExtent);

    TArray<AActor*> CollisionActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), CollisionActors);

    for (AActor* Actor : CollisionActors)
    {
        FVector Origin, Extent;
        Actor->GetActorBounds(false, Origin, Extent);
        FBox ActorBox(Origin - Extent, Origin + Extent);

        // 3D 불린 연산
        if (SpawnBox.Intersect(ActorBox))
        {
            return false; 
        }
    }

    return true;
}
