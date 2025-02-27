
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/HealingItem.h"
#include "Beer.generated.h"

UCLASS()
class PROJECT_GGF_API ABeer : public AHealingItem
{
	GENERATED_BODY()

public:
	virtual void UseItem() override;


public:	
	ABeer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
