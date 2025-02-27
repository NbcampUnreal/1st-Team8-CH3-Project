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
	
	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem* SmokeEffect;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
	class UParticleSystemComponent* SmokeParticle;

	UPROPERTY(EditAnywhere, Category = "Particle")
	float CurrentScale;
	
	UPROPERTY(EditAnywhere, Category = "Particle")
	float MaxScale;
	
	UPROPERTY(EditAnywhere, Category = "Particle")
	float GrowSpeed;

	FTimerHandle GrowTimer;
	FTimerHandle DownTimer;

	FTimerHandle DestroyTimer;


protected:
	UFUNCTION()
	void GrowSmoke(); 

	UFUNCTION()
	void DestroySmoke();  


	virtual void Activation() override;


public:	
	ASmokeGrenade();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
