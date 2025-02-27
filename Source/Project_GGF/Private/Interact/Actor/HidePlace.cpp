#include "Interact/Actor/HidePlace.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Project_GGFCharacter.h"
#include "Engine/World.h"

AHidePlace::AHidePlace()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Interactive Actor"));
    RootComponent = Root;
    
    ShelterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShelterMesh"));
    ShelterMeshComponent->SetupAttachment(RootComponent);

    NotifyInteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    NotifyInteractionWidget->SetupAttachment(RootComponent);
    NotifyInteractionWidget->SetVisibility(false);

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
    CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); 
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);  
    CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
}

void AHidePlace::BeginPlay()
{
    Super::BeginPlay();
}



void AHidePlace::ShowInteractionWidget(bool bShow)
{
    if (NotifyInteractionWidget)
    {
        NotifyInteractionWidget->SetVisibility(bShow); 
    }
}

void AHidePlace::InteractionKeyPressed(AActor* Actor)
{
    AProject_GGFCharacter* Character = Cast<AProject_GGFCharacter>(Actor);
    if (Character)
    {
        if (!bIsInsideShelter)
        {
            EnterShelter(Actor);  
        }
        else
        {
            ExitShelter(Actor);  
        }
    }
}

void AHidePlace::EnterShelter(AActor* Actor)
{
    AProject_GGFCharacter* Character = Cast<AProject_GGFCharacter>(Actor);
    if (Character)
    {
        LastCharacterLocation = Character->GetActorLocation();

        Character->SetActorEnableCollision(false);  
        FVector ShelterLocation = GetActorLocation() + FVector(0, 0, 200);  
        Character->SetActorLocation(ShelterLocation);
        Character->SetActorHiddenInGame(true);  
        Character->GetCharacterMovement()->SetMovementMode(MOVE_None);  
        bIsInsideShelter = true;

        ShowInteractionWidget(false);  
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

        ShowInteractionWidget(true); 
    }
}
