#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "Shotgun.generated.h"

UCLASS()
class PROJECT_GGF_API AShotgun : public ARangedWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	float SpreadAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon/Type")
	int32 ShotAmmoCnt;

public:
	virtual bool Shot() override;
	virtual bool Reloading(int32 _TotalAmmo) override;

public:
	AShotgun();
};
