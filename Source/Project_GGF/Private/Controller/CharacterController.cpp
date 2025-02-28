// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_GGF/Public/Controller/CharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Items/Inventory/BackpackInventory.h"
#include "Items/Manager/ItemDataManager.h"
#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Items/Inventory/InventoryObject.h"

ACharacterController::ACharacterController()
    : InputMappingContext(nullptr),
	MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    QuietAction(nullptr),
    AimAction(nullptr),
    FireAction(nullptr),
    SprintAction(nullptr),
    ReloadAction(nullptr),
    SitAction(nullptr),
    ZoomAction(nullptr),
    ZoomScopeAction(nullptr),
    FirButtonAction(nullptr),
    SecButtonAction(nullptr),
    ThirdButtonAction(nullptr),
    FourthButtonAction(nullptr),
    InteractAction(nullptr),
    UnequipAction(nullptr)

{
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(TEXT("/Game/GGF/Character/GGF_Character/Input/IMC_Character")); // ��� Ȯ��
    if (IMCAsset.Succeeded())
    {
        InputMappingContext = IMCAsset.Object;
    }
}


void ACharacterController::BeginPlay()
{
    Super::BeginPlay();


    APlayerController* PlayerController = Cast<APlayerController>(this);
    if (PlayerController)
    {
        ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
        if (LocalPlayer)
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

}



UUserWidget* ACharacterController::CreateBackpackInventory(TSubclassOf<UUserWidget> _BackpackInventoryClass)
{
    if (_BackpackInventoryClass)
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(this, _BackpackInventoryClass);
        if (Widget)
        {
            return Widget;
        }
    }
    return nullptr;
}

void ACharacterController::ShowBackpackInventoryUI()
{
    AProject_GGFCharacter* PlayerCharacter = Cast<AProject_GGFCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (PlayerCharacter)
    {
        if (PlayerCharacter->GetInventoryObject())
        {
            if (PlayerCharacter->GetInventoryObject()->InventoryInstance)
            {
                PlayerCharacter->GetInventoryObject()->InventoryInstance->AddToViewport();
                bShowMouseCursor = true;
                SetInputMode(FInputModeUIOnly());
            }
        }
    }
}