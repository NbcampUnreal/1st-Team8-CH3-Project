// Fill out your copyright notice in the Description page of Project Settings.

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
	TArray<AWeapon*> Weapons;


public:
	bool Shot();
	bool Reload();


public:
	UWeaponManager();
	~UWeaponManager();

};
