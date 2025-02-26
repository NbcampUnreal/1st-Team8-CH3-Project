
#include "Interact/Actor/HidePlace.h"
#include "Interact/GGFInteractiveActor.h"
#include "Character/Project_GGFCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AHidePlace::AHidePlace()
{
	ShelterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShelterMesh"));
	ShelterMeshComponent->SetupAttachment(RootComponent);
	
}

void AHidePlace::BeginPlay()
{
	Super::BeginPlay();
}

void AHidePlace::InteractionKeyPressed(AProject_GGFCharacter* Character)
{
	Super::InteractionKeyPressed(Character);
	
	if (!bIsInsideShelter)  
	{
		
		EnterShelter(Character);  
	}
	else
	{
		
		ExitShelter(Character);
	}
}

void AHidePlace::EnterShelter(AActor* Actor)
{
	AProject_GGFCharacter* Character = Cast<AProject_GGFCharacter>(Actor);
	if (Character)
	{
		LastCharacterLocation = Character->GetActorLocation();
		
		Character->SetActorEnableCollision(false);
		FVector ShelterLocation = GetActorLocation() + FVector(0, 0, 50);
		Character->SetActorLocation(ShelterLocation);
		Character->SetActorHiddenInGame(true);
		Character->GetCharacterMovement()->SetMovementMode(MOVE_None);

		bIsInsideShelter = true;
		
		if (NotifyInteractionWidget)
		{
			NotifyInteractionWidget->SetVisibility(false);
		}
	}
}

void AHidePlace::ExitShelter(AActor* Actor)
{
	AProject_GGFCharacter* Character = Cast<AProject_GGFCharacter>(Actor);
	if (Character)
	{
		Character->SetActorLocation(LastCharacterLocation);
		Character->SetActorHiddenInGame(false);
		Character->SetActorEnableCollision(true);
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		bIsInsideShelter = false;
		
		if (NotifyInteractionWidget)
		{
			NotifyInteractionWidget->SetVisibility(true);
		}
	}
}

