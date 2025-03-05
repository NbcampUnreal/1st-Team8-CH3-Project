// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact/GGFInteractiveActor.h"
#include "DeadAIItemsInteractiveActor.generated.h"

class UInventoryObject;

UCLASS()
class PROJECT_GGF_API ADeadAIItemsInteractiveActor : public AGGFInteractiveActor
{
	GENERATED_BODY()
	
public:
	UInventoryObject* InventoryObjectInstance;

public:

	// 전리품 데이터 설정
	void SetInventoryData(UInventoryObject* InventoryObj);

	virtual void InteractionKeyPressed(AProject_GGFCharacter* Character) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)override;
};
