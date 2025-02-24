#include "Gameplay/GGFGameMode.h"
#include "Gameplay/GGFGameState.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


AGGFGameMode::AGGFGameMode()
{
	GameStateClass = AGGFGameState::StaticClass();
}

void AGGFGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!SpawnManager)
    {
        SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(ASpawnManager::StaticClass());
    }

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


void AGGFGameMode::SpawnAI(ESpawnType SpawnType, int32 Count)
{
    if (!SpawnManager) return;
	if (!AIClasses.Contains(SpawnType)) return;

	TSubclassOf<ACharacter> AIClass = AIClasses[SpawnType];
	if (!AIClass) return;

    int32 SpawnedCount = 0;

    while (SpawnedCount < Count && SpawnManager->GridSpawnPoints.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, SpawnManager->GridSpawnPoints.Num() - 1);
        FVector SpawnLocation = SpawnManager->GridSpawnPoints[Index];

        UE_LOG(LogTemp, Warning, TEXT("Spawning AI at: X=%f, Y=%f, Z=%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

        ACharacter* SpawnedAI = GetWorld()->SpawnActor<ACharacter>(AIClass, SpawnLocation, FRotator::ZeroRotator);
        if (SpawnedAI)
        {
            SpawnedCount++;
        }

        SpawnManager->GridSpawnPoints.RemoveAt(Index);
    }
}

