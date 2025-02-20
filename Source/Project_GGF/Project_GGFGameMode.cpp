// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_GGFGameMode.h"
#include "Project_GGFCharacter.h"
#include "AI/AICharacter.h"
#include "AI/AIControllerCustom.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AProject_GGFGameMode::AProject_GGFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AProject_GGFGameMode::BeginPlay()
{

	Super::BeginPlay();

	SpawnAIFromAllSpawnPoints();
}

void AProject_GGFGameMode::SpawnAIFromAllSpawnPoints()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "AISpawnPoint", SpawnPoints);

	for (AActor* SpawnPoint : SpawnPoints)
	{
		FVector SpawnLocation = SpawnPoint->GetActorLocation();
		FRotator SpawnRotation = SpawnPoint->GetActorRotation();

		SpawnAIAtLocation(SpawnLocation, SpawnRotation);
	}

}

void AProject_GGFGameMode::SpawnAIAtLocation(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!AICharacterClass || !AIControllerClass) return;

	AAICharacter* SpawnedAI = GetWorld()->SpawnActor<AAICharacter>(AICharacterClass, SpawnLocation, SpawnRotation);

	if (SpawnedAI)
	{
		AAIControllerCustom* AIController = GetWorld()->SpawnActor<AAIControllerCustom>(AIControllerClass, SpawnLocation, SpawnRotation);
		if (AIController)
		{
			AIController->Possess(SpawnedAI);
			UE_LOG(LogTemp, Warning, TEXT("AI가 %s 위치에서 생성"), *SpawnLocation.ToString());
		}
	}

}

