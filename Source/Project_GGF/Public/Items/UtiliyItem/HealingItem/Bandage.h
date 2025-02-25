
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/HealingItem.h"
#include "Bandage.generated.h"

UCLASS()
class PROJECT_GGF_API ABandage : public AHealingItem
{
	GENERATED_BODY()
	
public:
	virtual void UseItem() override;


public:	
	ABandage();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
