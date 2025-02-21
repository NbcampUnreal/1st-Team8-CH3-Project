#include "AI/NPC/Poacher.h"
#include "AI/NPC/PoacherController.h"

APoacher::APoacher()
{
	AIControllerClass = APoacherController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}