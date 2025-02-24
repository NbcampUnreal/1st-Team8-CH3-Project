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
	


protected:
	virtual void Activation() override;

	virtual void OnBulletOverlap(
		UPrimitiveComponent* _overlapComp,
		AActor* _otherActor,
		UPrimitiveComponent* _otherComp,
		int32 _otherBodyIndex,
		bool _bFromSweep,
		const FHitResult& _sweepResult
	);

public:	
	ADynamite();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
