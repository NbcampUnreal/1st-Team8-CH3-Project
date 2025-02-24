// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/UtiliyItem/HealingItem/Beer.h"

// Sets default values
ABeer::ABeer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABeer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

