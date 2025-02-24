// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/ThrowingItem.h"
#include "SmokeGrenade.generated.h"

UCLASS()
class PROJECT_GGF_API ASmokeGrenade : public AThrowingItem
{
	GENERATED_BODY()
	

protected:
	virtual void Activation() override;


public:	
	// Sets default values for this actor's properties
	ASmokeGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
