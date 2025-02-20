#include "Gameplay/GGFGameMode.h"
#include "Gameplay/GGFGameState.h"
#include "NavigationSystem.h"
#include "AI/Creatures/DeerDoe.h"

AGGFGameMode::AGGFGameMode()
{
	GameStateClass = AGGFGameState::StaticClass();
}

void AGGFGameMode::BeginPlay()
{
    Super::BeginPlay();

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
        UE_LOG(LogTemp, Error, TEXT("베드 엔딩"));
        // 게임 종료 구현하기
    }
}

void AGGFGameMode::SpawnAI(ESpawnType SpawnType, int32 Count)
{
	if (!AIClasses.Contains(SpawnType)) return;

	TSubclassOf<ACharacter> AIClass = AIClasses[SpawnType];
	if (!AIClass) return;

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return;

    for (int32 i = 0; i < Count; i++)
    {
		FNavLocation SpawnLocation;

		if (NavSystem->GetRandomReachablePointInRadius(FVector::ZeroVector, 5000.0f, SpawnLocation))
		{
			ACharacter* SpawnedAI = GetWorld()->SpawnActor<ACharacter>(AIClass, SpawnLocation.Location, FRotator::ZeroRotator);

			if (SpawnedAI)
			{
				SpawnedAI->SpawnDefaultController();

				//사슴 리더 설정
				if (SpawnType == ESpawnType::DeerDoe)
				{
					ADeerDoe* Deer = Cast<ADeerDoe>(SpawnedAI);
					if (Deer)
					{
						static bool bIsFirstDeer = true;
						Deer->bIsLeader = bIsFirstDeer;
						bIsFirstDeer = false;
					}
				}
			}
		}
    }
}

float AGGFGameMode::GetGameTimeRatio() const
{
    return FMath::Clamp(CurrentTime / TotalGameTime, 0.0f, 1.0f);
}