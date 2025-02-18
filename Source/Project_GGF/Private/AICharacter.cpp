#include "AICharacter.h"
#include "AIControllerCustom.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = AAIControllerCustom::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
