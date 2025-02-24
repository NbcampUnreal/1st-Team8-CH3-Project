// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/UtiliyItem/ItemBase.h"
#include "UtilityItem.generated.h"

UCLASS()
class PROJECT_GGF_API AUtilityItem : public AItemBase
{
	GENERATED_BODY()
	
protected:



public:	
	// Sets default values for this actor's properties
	AUtilityItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
