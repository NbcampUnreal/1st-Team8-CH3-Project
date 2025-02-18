#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponManager.generated.h"

class AWeapon;

UCLASS()
class PROJECT_GGF_API UWeaponManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<AWeapon*> Weapons;
	int32 CurrentIdx;

public:
	bool Attack();
	bool Reload();
	AWeapon ChangeWeapon(int32 _Idx);

	void AddWeapon(AActor* _Actor);
public:
	UWeaponManager();
	~UWeaponManager();

};
