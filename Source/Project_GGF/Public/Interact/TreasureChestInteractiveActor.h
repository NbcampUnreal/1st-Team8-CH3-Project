// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact/GGFInteractiveActor.h"
#include "TreasureChestInteractiveActor.generated.h"

class UInventoryObject;

UCLASS()
class PROJECT_GGF_API ATreasureChestInteractiveActor : public AGGFInteractiveActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventoryObject> InventoryObjectPtr;
	UInventoryObject* InventoryObjectInstance;

public:
	virtual void InteractionKeyPressed(AProject_GGFCharacter* Character) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

public:
	ATreasureChestInteractiveActor();

protected:
	virtual void BeginPlay() override;

	
};
