// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "Project_GGF/Public/Component/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Gameplay/Quest/QuestManager.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AProject_GGFCharacter::AProject_GGFCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationYaw = true; 

	
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	SpringArmComp->SocketOffset = FVector(15.3f, 68.0f, 71.3f);

	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComp); 
	FollowCamera->SetFieldOfView(90.0f);

	//Quiet
	QuietSpeedMultiplier = 0.5;
	QuietSpeed = GetCharacterMovement()->MaxWalkSpeed * QuietSpeedMultiplier;
	bIsQuiet = false;
	//Sit
	SitSpeedMultiplier = 0.7;
	SitSpeed = GetCharacterMovement()->MaxWalkSpeed * SitSpeedMultiplier;
	bIsSitting = false;
	//Sprint
	SprintSpeedMultiplier = 2.0f;
	SprintSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintSpeedMultiplier;
	bIsSprinting = false;
	//Stamina
	MaxStamina = 100.f;
	Stamina = MaxStamina;
	StaminaDrainRate = 10.0f;

	//Respawn
	RespawnDelay = 5.0f;


}


void AProject_GGFCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCameraFOV(90.0f);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	QuestManager = Cast<AQuestManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AQuestManager::StaticClass()));
}

//////////////////////////////////////////////////////////////////////////
// Input



void AProject_GGFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Look);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Reload);

		EnhancedInputComponent->BindAction(SitAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ToggleSit);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopAim);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopFire);

		EnhancedInputComponent->BindAction(QuietAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartQuiet);
		EnhancedInputComponent->BindAction(QuietAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopQuiet);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopSprint);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProject_GGFCharacter::Move(const FInputActionValue& Value)
{

	if (!Controller) return;

	const FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

	if (MoveInput.IsNearlyZero())
	{
		StartStaminaRecovery();
	}
	else
	{
		StopStaminaRecovery();
	}
}
	

void AProject_GGFCharacter::Look(const FInputActionValue& Value)
{
	
	FVector2D LookInput = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookInput.X);
		AddControllerPitchInput(LookInput.Y);
	}
}

/** Called for Sprint input */
void AProject_GGFCharacter::StartSprint(const FInputActionValue& Value)
{
	if (Stamina <= 0.0f)
	{
		return; 
	}

	bIsSprinting = true;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

	if (Stamina > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SprintStaminaHandle,  
			this,                 
			&AProject_GGFCharacter::UseStamina,  
			0.5f,                 
			true                  
		);
	}

	StopStaminaRecovery();

}
void AProject_GGFCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}

	GetWorld()->GetTimerManager().ClearTimer(SprintStaminaHandle);
	StartStaminaRecovery();
}

/** Called for Reload input */
void AProject_GGFCharacter::Reload(const FInputActionValue& Value)
{

}

/** Called for Sit input */
void AProject_GGFCharacter::ToggleSit(const FInputActionValue& Value)
{
	bIsSitting = !bIsSitting;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsSitting ? SitSpeed : 500.0f;
	}
}

/** Called for Aim input */
void AProject_GGFCharacter::StartAim(const FInputActionValue& Value)
{
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(50.0f); 
	}

	GetCharacterMovement()->MaxWalkSpeed *= 0.5f;
}
void AProject_GGFCharacter::StopAim(const FInputActionValue& Value)
{
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(90.0f);
	}

	GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
}

/** Called for Fire input */
void AProject_GGFCharacter::StartFire(const FInputActionValue& Value)
{

	StopStaminaRecovery();
}
void AProject_GGFCharacter::StopFire(const FInputActionValue& Value)
{

	StartStaminaRecovery();
}

/** Called for Quiet input */
void AProject_GGFCharacter::StartQuiet(const FInputActionValue& Value)
{
	bIsQuiet = true;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = QuietSpeed;
	}

	StopStaminaRecovery();
}
void AProject_GGFCharacter::StopQuiet(const FInputActionValue& Value)
{
	bIsQuiet = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

/// Stamina

void AProject_GGFCharacter::RestoreStamina()
{
	if (Stamina < MaxStamina)
	{
		Stamina = FMath::Clamp(Stamina + 5, 0.0f, MaxStamina);
		FString StaminaText = FString::Printf(TEXT("Stamina: %.0f / %.0f"), Stamina, MaxStamina);
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, StaminaText);
	}
}

void AProject_GGFCharacter::UseStamina()
{
	if (Stamina > 0)
	{
		Stamina -= StaminaDrainRate;
		Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
		FString StaminaText = FString::Printf(TEXT("Stamina: %.0f / %.0f"), Stamina, MaxStamina);
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, StaminaText);


	}

	if (Stamina <= 0.0f)
	{
		StopSprint(FInputActionValue());
	}
}

void AProject_GGFCharacter::StartStaminaRecovery()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(StaminaRestoreHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			StaminaRestoreHandle, 
			this,                 
			&AProject_GGFCharacter::RestoreStamina, 
			2.0f,                   
			true                    
		);
	}

}


void AProject_GGFCharacter::StopStaminaRecovery()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRestoreHandle);
}

/// Death

void AProject_GGFCharacter::OnDeath()
{
	UHealthComponent* HealthComp = FindComponentByClass<UHealthComponent>();
	if (!HealthComp || HealthComp->bIsDead)
		return;


	HealthComp->bIsDead = true;


	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}

	
	GetWorldTimerManager().SetTimer(
		TimerHandle_Respawn,
		this,
		&AProject_GGFCharacter::Respawn,
		RespawnDelay,
		false
	);
}

void AProject_GGFCharacter::Respawn()
{

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		FVector SpawnLocation = FVector(0, 0, 300); 
		FRotator SpawnRotation = FRotator::ZeroRotator;


		AActor* NewCharacter = GetWorld()->SpawnActor<AProject_GGFCharacter>(
			GetClass(), SpawnLocation, SpawnRotation
		);

		
		if (NewCharacter)
		{
			PlayerController->Possess(Cast<APawn>(NewCharacter));
		}
	}
}

void AProject_GGFCharacter::SetCameraFOV(float NewFOV)
{
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(NewFOV);
	}
}

void AProject_GGFCharacter::AddItemToInventory(FString ItemName, int32 Amount)
{
	if (QuestManager)
	{
		QuestManager->UpdateQuestProgress(ItemName, Amount);
	}

}
