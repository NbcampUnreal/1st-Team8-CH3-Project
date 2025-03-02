#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HidePlace.generated.h"

class UWidgetComponent;
class USceneComponent;
class AHidePlace;
class UBoxComponent;

UCLASS()
class PROJECT_GGF_API AHidePlace : public AActor
{
	GENERATED_BODY()

public:
	AHidePlace();

protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, Category = "Shelter")
	UStaticMeshComponent* ShelterMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetComponent* NotifyInteractionWidget;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* CollisionBox;

	bool bIsInsideShelter = false;

	FVector LastCharacterLocation;

	
	void ShowInteractionWidget(bool bShow);
	void InteractionKeyPressed(AActor* Actor);
	void EnterShelter(AActor* Actor);
	void ExitShelter(AActor* Actor);
	
};
