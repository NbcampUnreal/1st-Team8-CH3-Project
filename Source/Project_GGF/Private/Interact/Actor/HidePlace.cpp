#include "Interact/Actor/HidePlace.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Project_GGFCharacter.h"
#include "GameFramework/SpringArmComponent.h"
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
        
        FVector CharacterForwardDirection = Character->GetActorForwardVector();
        FVector ShelterLocation = Character->GetActorLocation() + CharacterForwardDirection * 250.0f; 
        
        
        Character->SetActorLocation(ShelterLocation);
        Character->ToggleSit();
        
        
        FVector ShelterCameraLocation = ShelterLocation + FVector(0.0f, 0.0f, 400.0f);  
        Character->FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        Character->FollowCamera->SetWorldLocation(ShelterCameraLocation);
        Character->FollowCamera->bUsePawnControlRotation = true;

      
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
        Character->SetActorEnableCollision(true);
        Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

        if (Character->FollowCamera)
        {
            Character->FollowCamera->AttachToComponent(Character->SpringArmComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
            Character->FollowCamera->bUsePawnControlRotation = true;
            Character->FollowCamera->SetRelativeLocation(FVector(100.0f, -20.0f, 10.0f));
        }

        Character->ToggleSit();
        
        Character->EndInteract();
        bIsInsideShelter = false;
        
        ShowInteractionWidget(true);
    }
}

