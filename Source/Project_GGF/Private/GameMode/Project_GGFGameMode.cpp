// Copyright Epic Games, Inc. All Rights Reserved.
#include "Project_GGF/Public/GameMode/Project_GGFGameMode.h"
#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Project_GGF/Public/Controller/CharacterController.h"
#include "UObject/ConstructorHelpers.h"

AProject_GGFGameMode::AProject_GGFGameMode()
{
	DefaultPawnClass = AProject_GGFCharacter::StaticClass();
	PlayerControllerClass = ACharacterController::StaticClass();
}
