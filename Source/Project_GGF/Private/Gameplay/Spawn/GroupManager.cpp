#include "Gameplay/Spawn/GroupManager.h"
#include "AI/GGFAICharacterBase.h"
#include "AI/GGFAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


AGroupManager::AGroupManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGroupManager::RegisterMember(AGGFAICharacterBase* Member)
{
	if (Member)
	{
		GroupMembers.Add(Member);
	}
}

void AGroupManager::TriggerRunAway(AGGFAICharacterBase* Initiator)
{
	for (AGGFAICharacterBase* Member : GroupMembers)
	{
		if (Member && Member != Initiator)
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(
				TimerHandle,
				FTimerDelegate::CreateUObject(this, &AGroupManager::TriggerGroupRunAway, Member),
				3.0f, 
				false
			);
		}
	}
}

void AGroupManager::TriggerGroupRunAway(AGGFAICharacterBase* Member)
{
	if (Member)
	{
		if (AGGFAIController* AIController = Cast<AGGFAIController>(Member->GetController()))
		{
			if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
			{
				Blackboard->SetValueAsBool(TEXT("bRunAway"), true);
			}
		}
	}
}
