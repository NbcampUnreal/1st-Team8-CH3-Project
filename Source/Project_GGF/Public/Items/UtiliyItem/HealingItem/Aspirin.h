// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/HealingItem.h"
#include "Aspirin.generated.h"

UCLASS()
class PROJECT_GGF_API AAspirin : public AHealingItem
{
	GENERATED_BODY()
	
public:
	virtual void UseItem() override;

public:	
	AAspirin();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
