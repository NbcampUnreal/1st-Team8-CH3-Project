
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/HealingItem.h"
#include "Coffee.generated.h"

UCLASS()
class PROJECT_GGF_API ACoffee : public AHealingItem
{
	GENERATED_BODY()

public:
	virtual void UseItem() override;
	
public:	
	ACoffee();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
