#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroupManager.generated.h"

class AGGFAICharacterBase;

UCLASS()
class PROJECT_GGF_API AGroupManager : public AActor
{
	GENERATED_BODY()

public:
	AGroupManager();

	void RegisterMember(AGGFAICharacterBase* Member);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Group")
	TArray<AGGFAICharacterBase*> GroupMembers;
	
	void TriggerRunAway(AGGFAICharacterBase* Initiator);

private:
	void TriggerGroupRunAway(AGGFAICharacterBase* Member);
};
