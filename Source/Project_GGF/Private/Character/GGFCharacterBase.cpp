#include "Character/GGFCharacterBase.h"

AGGFCharacterBase::AGGFCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGGFCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGGFCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGGFCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

