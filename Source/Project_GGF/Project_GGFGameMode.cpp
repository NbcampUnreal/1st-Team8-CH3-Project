// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_GGFGameMode.h"
#include "Project_GGFCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_GGFGameMode::AProject_GGFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
