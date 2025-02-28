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
   
        
        FVector CameraFixedLocation = Character->FollowCamera->GetComponentLocation();

        
        Character->SetActorEnableCollision(false);
        FVector ShelterLocation = GetActorLocation() + FVector(0, 0, 30);
        Character->SetActorLocation(ShelterLocation);
        Character->GetCharacterMovement()->SetMovementMode(MOVE_None);
        bIsInsideShelter = true;

        if (Character->FollowCamera)
        {
            Character->FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            Character->FollowCamera->SetWorldLocation(CameraFixedLocation);
            Character->FollowCamera->bUsePawnControlRotation = true; 
        }
        
        APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
        if (PlayerController)
        {
            PlayerController->SetIgnoreMoveInput(true); 
            PlayerController->SetIgnoreLookInput(false);  
        }

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

        
        APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
        if (PlayerController)
        {
            PlayerController->SetIgnoreMoveInput(false); 
            PlayerController->SetIgnoreLookInput(false);  
        }

        bIsInsideShelter = false;

        ShowInteractionWidget(true);
    }
}

