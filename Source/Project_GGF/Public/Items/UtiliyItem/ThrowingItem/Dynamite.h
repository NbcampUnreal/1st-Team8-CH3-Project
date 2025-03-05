// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/ThrowingItem.h"
#include "Dynamite.generated.h"

UCLASS()
class PROJECT_GGF_API ADynamite : public AThrowingItem
{
	GENERATED_BODY()
	
public:
	virtual void Throw(FVector LaunchVelocity) override;


protected:
	virtual void Activation() override;

public:	
	ADynamite();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
