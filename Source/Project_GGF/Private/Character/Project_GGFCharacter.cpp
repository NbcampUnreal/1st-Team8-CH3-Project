// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
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
#include "Gameplay/Quest/QuestManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AProject_GGFCharacter::AProject_GGFCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(22.0f, 96.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	CharacterMesh = GetMesh();
	CharacterMesh->SetupAttachment(RootComponent);
	CharacterMesh->SetVisibility(true);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CharacterMesh);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(25.0f, 68.0f, 71.3f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComp);
	FollowCamera->SetFieldOfView(90.0f);
	FollowCamera->SetRelativeLocation(FVector(100.0f, -20.0f, 10.0f));


	CurrentWeapon = nullptr;


	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	RespawnComp = CreateDefaultSubobject<URespawnComponent>(TEXT("RespawnComponent"));
	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	NoiseComp = CreateDefaultSubobject<UNoiseComponent>(TEXT("NoiseComponent"));

	///Speed
	MaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SpeedBoostDuration = 5.0f;
	SpeedBoostMultiplier = 1.5f;



	//Quiet
	QuietSpeedMultiplier = 0.5;
	QuietSpeed = GetCharacterMovement()->MaxWalkSpeed * QuietSpeedMultiplier;
	bIsQuiet = false;
	//Sit
	SitSpeedMultiplier = 0.4;
	SitSpeed = GetCharacterMovement()->MaxWalkSpeed * SitSpeedMultiplier;
	bIsSitting = false;
	//Sprint
	SprintSpeedMultiplier = 2.0f;
	SprintSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintSpeedMultiplier;
	bIsSprinting = false;

	//camera
	
	DefaultFOV = 90.0f;  
	AimFOV = 50.0f;      
	ZoomInterpSpeed = 10.0f;
	CurrentFOV = DefaultFOV;
	MinFOV = 22.5f;       
	MaxFOV = 90.0f;     
	ZoomStep = 20.0f;
	
	//Throw
	ThrowStrength = 1000.0f;
}


void AProject_GGFCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	WeaponManager = Cast<UWeaponManager>(WeaponManagerPtr.GetDefaultObject());

	if (WeaponManager)
	{
		WeaponManager->CreateWeapons(this);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input



void AProject_GGFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Look);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Reload);

		EnhancedInputComponent->BindAction(SitAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ToggleSit);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopAim);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ToggleZoom);

		EnhancedInputComponent->BindAction(ZoomScopeAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ZoomScope);
	
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::StartFire);
		
		EnhancedInputComponent->BindAction(QuietAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartQuiet);
		EnhancedInputComponent->BindAction(QuietAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopQuiet);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopSprint);

		EnhancedInputComponent->BindAction(FirButtonAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::FirstButtonAction);
		EnhancedInputComponent->BindAction(SecButtonAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::SecondButtonAction);

		//EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Interact);
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
		StaminaComp->StartStaminaRecovery();
		NoiseComp->StopNoiseTimer();
	}
	else
	{
		StaminaComp->StopStaminaRecovery();

		if (GetWorld()->GetTimerManager().IsTimerActive(NoiseComp->NoiseTimerHandle))
		{
			NoiseComp->StopNoiseTimer();
		}

		NoiseComp->NoiseIntensity = NoiseComp->AverageIntensity;
		NoiseComp->NoiseRadius = NoiseComp->AverageRadiuse;
		NoiseComp->StartNoiseTimer(NoiseComp->NoiseIntensity, NoiseComp->NoiseRadius);

	}
}
	

void AProject_GGFCharacter::Look(const FInputActionValue& Value)
{
	
	FVector2D LookInput = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
	
		AddControllerPitchInput(LookInput.Y);

		TargetYaw = GetControlRotation().Yaw - GetActorRotation().Yaw;
		UpperBodyYaw = FMath::ClampAngle(TargetYaw, -90.f, 90.f);

		TargetPitch = GetControlRotation().Pitch - GetActorRotation().Pitch;
		UpperBodyPitch = FMath::ClampAngle(TargetPitch, -45.f, 45.f);

		FRotator NewRotation = GetControlRotation();
		NewRotation.Yaw += LookInput.X; 
		Controller->SetControlRotation(NewRotation);
	}
}

																									/** Called for Sprint input */
void AProject_GGFCharacter::StartSprint(const FInputActionValue& Value)
{
	
	if (bIsSitting)
	{
		return;
	}

	if (bIsFirstPerson)
	{
		ToggleZoom(Value);
	}
	if (bIsAiming)
	{
		StopAim();
	}

	if (StaminaComp->GetStamina() <= 0 || !GetCharacterMovement())
	{
		StopSprint();
		return;
	}

	bIsSprinting = true;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		StaminaComp->StopStaminaRecovery();

		GetWorld()->GetTimerManager().SetTimer(
			SprintStaminaHandle,
			StaminaComp,
			&UStaminaComponent::UseStamina,
			0.5f,  
			true
		);

	
		if (GetWorld()->GetTimerManager().IsTimerActive(NoiseComp->NoiseTimerHandle))
		{
			NoiseComp->StopNoiseTimer();
		}

		NoiseComp->NoiseIntensity = 100.0f;
		NoiseComp->NoiseRadius = 750.0f;
		NoiseComp->NoiseDelay = 0.1f;
		NoiseComp->StartNoiseTimer(NoiseComp->NoiseIntensity, NoiseComp->NoiseRadius);
	}
}


void AProject_GGFCharacter::StopSprint()
{
	bIsSprinting = false;

	
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	}

	
	GetWorld()->GetTimerManager().ClearTimer(SprintStaminaHandle);

	
	StaminaComp->StartStaminaRecovery();

	NoiseComp->NoiseDelay = 0.25f;
	NoiseComp->StopNoiseTimer();
}

																								/** Called for Reload input */
void AProject_GGFCharacter::Reload(const FInputActionValue& Value)
{
	if (!bIsArmed)
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("No weapon equipped!"));
		return;
	}
	else
	{
		if (WeaponManager)
		{
			WeaponManager->Reload();
		}
	}
}

																									/** Called for Sit input */
void AProject_GGFCharacter::ToggleSit(const FInputActionValue& Value)
{
	bIsSitting = !bIsSitting;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsSitting ? SitSpeed : MaxSpeed;
		NoiseComp->NoiseIntensity = 150.0f;
		NoiseComp->NoiseRadius = 10.0f;
	}

}

																									/** Called for Aim input */
void AProject_GGFCharacter::StartAim(const FInputActionValue& Value)
{
	bIsAiming = true;

	TargetFOV = AimFOV;
	GetWorld()->GetTimerManager().SetTimer(
		ZoomTimerHandle,
		this,
		&AProject_GGFCharacter::SetCameraFOV,
		0.01f,
		true);

	GetCharacterMovement()->MaxWalkSpeed *= 0.5f;
}
void AProject_GGFCharacter::StopAim()
{
	bIsAiming = false;

	TargetFOV = DefaultFOV;
	GetWorld()->GetTimerManager().SetTimer(
		ZoomTimerHandle,
		this,
		&AProject_GGFCharacter::SetCameraFOV,
		0.01f,
		true);

	GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
}

																									/** Called for Zoom input */
void AProject_GGFCharacter::ToggleZoom(const FInputActionValue& Value)
{
	bIsFirstPerson = !bIsFirstPerson;

	if (bIsFirstPerson)
	{
		SpringArmComp->bUsePawnControlRotation = false; 
		SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); 
		SpringArmComp->TargetArmLength = 0.0f; 

		
		FollowCamera->SetFieldOfView(90.0f); 
		FollowCamera->bUsePawnControlRotation = true;
		FollowCamera->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("head"));
		FollowCamera->SetRelativeLocation(FVector(15.0f, 0.0f, 0.0f));

	}
	else
	{
		SpringArmComp->bEnableCameraLag = true; 
		SpringArmComp->TargetArmLength = 300.0f;
		SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f)); 

		
		FollowCamera->SetFieldOfView(90.0f); 
		FollowCamera->bUsePawnControlRotation = false; 
		FollowCamera->AttachToComponent(SpringArmComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
		FollowCamera->SetRelativeLocation(FVector(100.0f, -20.0f, 10.0f));
	
	}
}


void AProject_GGFCharacter::ZoomScope(const FInputActionValue& Value)
{

	InputValue = Value.Get<float>();

	if (bIsFirstPerson)
	{
		
		CurrentFOV += InputValue * ZoomStep;

		CurrentFOV = FMath::Clamp(CurrentFOV, MinFOV, MaxFOV);

		FollowCamera->SetFieldOfView(CurrentFOV);
	}
}


																									/** Called for Fire input */
void AProject_GGFCharacter::StartFire(const FInputActionValue& Value)
{
	if (bIsArmed)
	{
		if (WeaponManager)
		{
			WeaponManager->Attack();
		}


		NoiseComp->NoiseIntensity = 200.0f;
		NoiseComp->NoiseRadius = 1500.0f;
		NoiseComp->GenerateNoiseTimer();
	}

	if (EquippedThrowableItem)
	{

	}
}

																									/** Called for Quiet input */
void AProject_GGFCharacter::StartQuiet(const FInputActionValue& Value)
{
	bIsQuiet = true;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = QuietSpeed;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(NoiseComp->NoiseTimerHandle))
	{
		NoiseComp->StopNoiseTimer();
	}

	NoiseComp->NoiseIntensity = 10.0f;
	NoiseComp->NoiseRadius = 200.0f;
	NoiseComp->StartNoiseTimer(NoiseComp->NoiseIntensity, NoiseComp->NoiseRadius);

	StaminaComp->StopStaminaRecovery();
}
void AProject_GGFCharacter::StopQuiet()
{
	bIsQuiet = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	}

	NoiseComp->StopNoiseTimer();
}

void AProject_GGFCharacter::FirstButtonAction(const FInputActionValue& Value)
{
	bIsArmed = !bIsArmed;

	if (bIsArmed)
	{
		if (WeaponManager)
		{
			WeaponManager->ChangeWeapon(1);
		}
	}
	else
	{
		if (WeaponManager)
		{
			WeaponManager->ChangeWeapon(0); // 무기 숨기기 또는 맨손 상태로 전환
		}
	}
}


void AProject_GGFCharacter::SecondButtonAction(const FInputActionValue & Value)
{
	bIsArmed = !bIsArmed;

	if (bIsArmed)
	{
		if (WeaponManager)
		{
			WeaponManager->ChangeWeapon(2);
		}
	}
	else
	{
		if (WeaponManager)
		{
			WeaponManager->ChangeWeapon(0); // 무기 숨기기 또는 맨손 상태로 전환
		}
	}
}

void AProject_GGFCharacter::Interact(const FInputActionValue& Value)
{

}




																												/// Camrera
void AProject_GGFCharacter::SetCameraFOV()
{
	CurrentFOV = FollowCamera->FieldOfView;
	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed);
	FollowCamera->SetFieldOfView(NewFOV);

	if (FMath::IsNearlyEqual(CurrentFOV, TargetFOV, 0.1f))
	{
		FollowCamera->SetFieldOfView(TargetFOV);
		GetWorld()->GetTimerManager().ClearTimer(ZoomTimerHandle);
	}
}

void AProject_GGFCharacter::AddItemToInventory(FString ItemName, int32 Amount)
{
	if (QuestManager)
	{
		QuestManager->UpdateQuestProgress(ItemName, Amount);
	}

}


void AProject_GGFCharacter::ActivateSpeedBoost()
{
	if (GetCharacterMovement())
	{
		
		GetCharacterMovement()->MaxWalkSpeed *= SpeedBoostMultiplier;
		GetWorld()->GetTimerManager().SetTimer(
			SpeedBoostTimerHandle,
			this,
			&AProject_GGFCharacter::ResetSpeedBoost,
			SpeedBoostDuration,
			false 
		);
	}
}

void AProject_GGFCharacter::ResetSpeedBoost()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed /= SpeedBoostMultiplier;
	}
}