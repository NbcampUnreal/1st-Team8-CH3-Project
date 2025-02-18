#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Weapon/RangedWeapon.h"
#include "HuntingRifle.generated.h"

UCLASS()
class PROJECT_GGF_API AHuntingRifle : public ARangedWeapon
{
	GENERATED_BODY()
	
public:	
	virtual bool Shot() override;
	virtual bool Reloading(int32 _TotalAmmo) override;

public:
	AHuntingRifle();


};
