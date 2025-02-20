// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_GGF/Public/Controller/CharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

ACharacterController::ACharacterController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    AimAction(nullptr),
    FireAction(nullptr),
    QuietAction(nullptr),
    ReloadAction(nullptr),
    SitAction(nullptr)
{
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(TEXT("/Game/GGF/Character/GGF_Character/Input/IMC_Character")); // 경로 확인 필수
    if (IMCAsset.Succeeded())
    {
        InputMappingContext = IMCAsset.Object;
    }
}

void ACharacterController::BeginPlay()
{
    Super::BeginPlay();


    if (InputMappingContext)
    {
        if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}


