#include "AICharacter.h"
#include "AIControllerCustom.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = AAIControllerCustom::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}
