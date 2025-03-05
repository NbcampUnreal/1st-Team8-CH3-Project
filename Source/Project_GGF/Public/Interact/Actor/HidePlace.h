#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact/GGFInteractiveActor.h"
#include "HidePlace.generated.h"



UCLASS()
class PROJECT_GGF_API AHidePlace : public AGGFInteractiveActor
{
	GENERATED_BODY()

public:
	AHidePlace();

protected:
	virtual void BeginPlay() override;

public:
	
	
	UPROPERTY(VisibleAnywhere, Category = "Shelter")
	UStaticMeshComponent* ShelterMeshComponent;
	

	bool bIsInsideShelter = false;

	FVector LastCharacterLocation;

	
	void ShowInteractionWidget(bool bShow);
	void InteractionKeyPressed(AActor* Actor);
	void EnterShelter(AActor* Actor);
	void ExitShelter(AActor* Actor);
	
};
