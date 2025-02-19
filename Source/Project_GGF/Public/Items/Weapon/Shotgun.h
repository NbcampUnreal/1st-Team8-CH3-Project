#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "Shotgun.generated.h"

UCLASS()
class PROJECT_GGF_API AShotgun : public ARangedWeapon
{
	GENERATED_BODY()
	
public:	
	AShotgun();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
