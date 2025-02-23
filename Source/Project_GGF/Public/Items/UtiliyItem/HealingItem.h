#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/UtilityItem.h"
#include "HealingItem.generated.h"

UCLASS()
class PROJECT_GGF_API AHealingItem : public AUtilityItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	EHealingType HealingType;

	UPROPERTY(VisibleAnywhere)
	float Amount;

	UPROPERTY(VisibleAnywhere)
	float Duration;

public:	
	AHealingItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
