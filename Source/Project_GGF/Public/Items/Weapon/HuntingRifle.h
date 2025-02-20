#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "HuntingRifle.generated.h"

class AHuntingRifleBullet;

UCLASS()
class PROJECT_GGF_API AHuntingRifle : public ARangedWeapon
{
	GENERATED_BODY()
	
protected:

public:	
	virtual bool Shot() override;
	virtual bool Reloading(int32 _TotalAmmo) override;

public:
	AHuntingRifle();
};
