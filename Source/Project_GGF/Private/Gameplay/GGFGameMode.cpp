#include "Gameplay/GGFGameMode.h"
#include "Gameplay/GGFGameState.h"
#include "GameFramework/Character.h"
#include "Gameplay/Spawn/SpawnManager.h"
#include "Gameplay/Spawn/SpawnVolume.h"
#include "Interact/LootInteractionActor.h"

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
    
    for (auto& SpawnInfo : SpawnInfoMap)
    {
        ECharacterType AnimalType = SpawnInfo.Key;
        FSpawnInfo Info = SpawnInfo.Value;
        
        SpawnAI(AnimalType, Info.Count, Info.GroupCount);
    }

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


FHealthData* AGGFGameMode::GetCharacterStat(ECharacterType type)
{
    int IntType = (int)type;
    FName StringType = *FString::FromInt(IntType);

    return CharacterStatTable->FindRow<FHealthData>(StringType, TEXT(""));
}

ECharacterType AGGFGameMode::GetCharacterType(TSubclassOf<ACharacter> CharacterClass)
{
    for (const auto& Pair : SpawnInfoMap)
    {
        if (Pair.Value.CharacterClass == CharacterClass)
        {
            return Pair.Key;
        }
    }
    return ECharacterType::Character;
}

void AGGFGameMode::SpawnAI(ECharacterType SpawnType, int32 Count, int32 GroupCount)
{
    if (!SpawnManager) return;
	if (!SpawnInfoMap.Contains(SpawnType)) return;

	TSubclassOf<ACharacter> AIClass = SpawnInfoMap[SpawnType].CharacterClass;
	if (!AIClass) return;

    if (GroupCount <= 0)
    {
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
    else
    {
        int32 GroupSize = Count / GroupCount;
        int32 GroupsSpawned = 0;

        while (GroupsSpawned < GroupCount && SpawnManager->GridSpawnPoints.Num() > 0)
        {
            int32 Index = FMath::RandRange(0, SpawnManager->GridSpawnPoints.Num() - 1);
            FVector SpawnLocation = SpawnManager->GridSpawnPoints[Index];

            UE_LOG(LogTemp, Warning, TEXT("Spawning Group at: X=%f, Y=%f, Z=%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
            
            ASpawnVolume* SpawnVolume = GetWorld()->SpawnActor<ASpawnVolume>(SpawnVolumeClass, SpawnLocation, FRotator::ZeroRotator);
            if (SpawnVolume)
            {
                SpawnVolume->GroupSpawnClass = AIClass;
                SpawnVolume->GroupSize = GroupSize;
                SpawnVolume->SpawnGroup();
            }

            GroupsSpawned++;
            SpawnManager->GridSpawnPoints.RemoveAt(Index);
        }
    }
}

void AGGFGameMode::SpawnLootInteractionActor(const FVector& Location, const TArray<FAnimalLoot>& Loot)
{
    if (!LootInteractionClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    ALootInteractionActor* LootActor = World->SpawnActor<ALootInteractionActor>(LootInteractionClass, Location, FRotator::ZeroRotator);
    if (LootActor)
    {
        LootActor->SetLootData(Loot);
    }
}

