#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Weapon/Weapon.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class PROJECT_GGF_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:	
	AMeleeWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
