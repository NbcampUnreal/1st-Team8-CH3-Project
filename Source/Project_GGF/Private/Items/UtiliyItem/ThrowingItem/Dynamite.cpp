// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/UtiliyItem/ThrowingItem/Dynamite.h"

// Sets default values
ADynamite::ADynamite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADynamite::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADynamite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

