// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Project_GGF/Public/Component/HealthComponent.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Items/Weapon/HuntingRifle.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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

	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

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
																										/** Called for Move input */
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

		StopNoiseTimer();
	}
	else
	{
		StopStaminaRecovery();

		if (GetWorld()->GetTimerManager().IsTimerActive(NoiseTimerHandle))
		{
			StopNoiseTimer();
		}

		float NoiseIntensity = bIsSitting ? 10.0f : 50.0f;  // 앉았을 때는 소리가 약하게
		float NoiseRadius = bIsSitting ? 100.0f : 500.0f;   // 앉았을 때는 범위 좁게

		GetWorld()->GetTimerManager().SetTimer(
			NoiseTimerHandle,
			[this, NoiseIntensity, NoiseRadius]() { GenerateNoiseTimer(NoiseIntensity, NoiseRadius); },
			0.25f,
			true);

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

	if (GetWorld()->GetTimerManager().IsTimerActive(NoiseTimerHandle))
	{
		StopNoiseTimer();
	}


	GetWorld()->GetTimerManager().SetTimer(
		NoiseTimerHandle,
		[this]() { GenerateNoiseTimer(100.0f, 750.0f);},
		0.1f,
		true);

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
	StopNoiseTimer();
}

																								/** Called for Reload input */
void AProject_GGFCharacter::Reload(const FInputActionValue& Value)
{
	if (WeaponManager)
	{
		WeaponManager->Reload();
	}
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
    if (WeaponManager)
    {
       WeaponManager->Attack();
    }


	FVector NoiseLocation = GetActorLocation();
	GenerateNoise(NoiseLocation, 200.f, 1500.0f);

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

	if (GetWorld()->GetTimerManager().IsTimerActive(NoiseTimerHandle))
	{
		StopNoiseTimer();
	}


	GetWorld()->GetTimerManager().SetTimer(
		NoiseTimerHandle,
		[this]() { GenerateNoiseTimer(10.0f, 200.0f);},
		0.25f,
		true);

	StopStaminaRecovery();
}
void AProject_GGFCharacter::StopQuiet(const FInputActionValue& Value)
{
	bIsQuiet = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}

	StopNoiseTimer();
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


																													//noise

void AProject_GGFCharacter::GenerateNoise(FVector NoiseLocation, float Intensity, float Radius)
{
	// FNoise 구조체를 생성
	FNoise NewNoise;
	NewNoise.Location = NoiseLocation;
	NewNoise.Intensity = Intensity;
	NewNoise.Radius = Radius;

}

void AProject_GGFCharacter::GenerateNoiseTimer(float Intensity, float Radius)
{
	FVector NoiseLocation = GetActorLocation();
	GenerateNoise(NoiseLocation, Intensity, Radius);
}

void AProject_GGFCharacter::StopNoiseTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(NoiseTimerHandle);
}


																												/// Camrera
void AProject_GGFCharacter::SetCameraFOV(float NewFOV)
{
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(NewFOV);
	}
}