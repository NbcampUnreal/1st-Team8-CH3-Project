
#pragma once

#include "CoreMinimal.h"
#include "Interact/GGFInteractiveActor.h"
#include "HidePlace.generated.h"

UCLASS()
class PROJECT_GGF_API AHidePlace : public AGGFInteractiveActor
{
	GENERATED_BODY()

public:

	AHidePlace();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shelter")
	UStaticMeshComponent* ShelterMeshComponent;

	void InteractionKeyPressed(AProject_GGFCharacter* Character) override;
	
	void EnterShelter(AActor* Actor);
	void ExitShelter(AActor* Actor);

	FVector LastCharacterLocation;
	bool bIsInsideShelter = false;
};
