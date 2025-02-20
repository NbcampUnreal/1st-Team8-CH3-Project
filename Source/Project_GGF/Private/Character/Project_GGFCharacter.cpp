// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Project_GGF/Public/Component/HealthComponent.h"
#include "Project_GGF/Public/Component/StaminaComponent.h"
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
	: WeaponManager(nullptr)
{

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetActive(false); 
	FirstPersonCamera->SetFieldOfView(90.0f);



	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetOwnerNoSee(true);
	


	CurrentWeapon = nullptr;

	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));

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

	//camera
	
	DefaultFOV = 90.0f;  // �⺻ ����
	AimFOV = 50.0f;      // ���� ����
	ZoomInterpSpeed = 10.0f;
	CurrentFOV = 90.0f;   // �⺻ FOV
	MinFOV = 22.5f;       
	MaxFOV = 90.0f;     
	ZoomStep = 15.0f;
	

}


void AProject_GGFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponManager = Cast<UWeaponManager>(WeaponManagerPtr.GetDefaultObject());

	if (WeaponManager)
		WeaponManager->CreateWeapons(this);
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

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ToggleZoom);

		EnhancedInputComponent->BindAction(ZoomScopeAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ZoomScope);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopFire);

		EnhancedInputComponent->BindAction(QuietAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartQuiet);
		EnhancedInputComponent->BindAction(QuietAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopQuiet);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopSprint);

		EnhancedInputComponent->BindAction(FirButtonAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::FirstButtonAction);
		EnhancedInputComponent->BindAction(SecButtonAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::SecondButtonAction);
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

		StopNoiseTimer();
	}
	else
	{
		StaminaComp->StopStaminaRecovery();

		if (GetWorld()->GetTimerManager().IsTimerActive(NoiseTimerHandle))
		{
			StopNoiseTimer();
		}

		NoiseIntensity = bIsSitting ? 10.0f : 50.0f;
		NoiseRadius = bIsSitting ? 100.0f : 500.0f;

		GetWorld()->GetTimerManager().SetTimer(
			NoiseTimerHandle,
			[this]() { GenerateNoiseTimer(NoiseIntensity, NoiseRadius);},
			0.25f,
			true);

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

	bIsSprinting = true;

	if (StaminaComp->GetStamina() > 0 && GetCharacterMovement())
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

		if (GetWorld()->GetTimerManager().IsTimerActive(NoiseTimerHandle))
		{
			StopNoiseTimer();
			GetWorld()->GetTimerManager().SetTimer(
				NoiseTimerHandle,
				[this]() { GenerateNoiseTimer(100.0f, 750.0f);},
				0.1f,
				true);
		}
	}
	else 
	{
		StopSprint(FInputActionValue());
	}
}
void AProject_GGFCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
		GetWorld()->GetTimerManager().ClearTimer(SprintStaminaHandle);
		StaminaComp->StartStaminaRecovery();
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
	TargetFOV = AimFOV;
	GetWorld()->GetTimerManager().SetTimer(
		ZoomTimerHandle,
		this,
		&AProject_GGFCharacter::SetCameraFOV,
		0.01f,
		true);

	GetCharacterMovement()->MaxWalkSpeed *= 0.5f;
}
void AProject_GGFCharacter::StopAim(const FInputActionValue& Value)
{
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
	if (CurrentCameraMode == ECameraMode::ThirdPerson)
	{
		CurrentCameraMode = ECameraMode::FirstPerson;

		// 1��Ī ��� Ȱ��ȭ
		FirstPersonCamera->SetActive(true);
		FollowCamera->SetActive(false);
		SpringArmComp->SetRelativeLocation(FVector(0, 0, 60));  // 1��Ī ī�޶� ��ġ

		// �޽� ����
		ThirdPersonMesh->SetOwnerNoSee(true);  // 3��Ī �޽� ����
		FirstPersonMesh->SetOwnerNoSee(false); // 1��Ī �޽� ���̱�
	}
	else
	{
		CurrentCameraMode = ECameraMode::ThirdPerson;

		// 3��Ī ��� Ȱ��ȭ
		FirstPersonCamera->SetActive(false);
		FollowCamera->SetActive(true);
		SpringArmComp->SetRelativeLocation(FVector(0, 0, 0));  // 3��Ī ī�޶� ��ġ �ʱ�ȭ

		// �޽� ����
		ThirdPersonMesh->SetOwnerNoSee(false); // 3��Ī �޽� ���̱�
		FirstPersonMesh->SetOwnerNoSee(true);  // 1��Ī �޽� ����
	}
}


void AProject_GGFCharacter::ZoomScope(const FInputActionValue& Value)
{
	
	InputValue = Value.Get<float>();

	if (CurrentCameraMode == ECameraMode::FirstPerson)
	{
		
		CurrentFOV += InputValue * ZoomStep;

		CurrentFOV = FMath::Clamp(CurrentFOV, MinFOV, MaxFOV);

		FirstPersonCamera->SetFieldOfView(CurrentFOV);
	}
}


																									/** Called for Fire input */
void AProject_GGFCharacter::StartFire(const FInputActionValue& Value)
{
	if (WeaponManager == nullptr)
		return;

    if (WeaponManager)
    {
       WeaponManager->Attack();
    }


	FVector NoiseLocation = GetActorLocation();
	GenerateNoise(NoiseLocation, 200.f, 1500.0f);

	StaminaComp->StopStaminaRecovery();
}
void AProject_GGFCharacter::StopFire(const FInputActionValue& Value)
{

	StaminaComp->StartStaminaRecovery();
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

	StaminaComp->StopStaminaRecovery();
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

void AProject_GGFCharacter::FirstButtonAction(const FInputActionValue & Value)
{
	bIsArmed = !bIsArmed;
	
	if (WeaponManager)
	{
		WeaponManager->ChangeWeapon(1);
	}
}


void AProject_GGFCharacter::SecondButtonAction(const FInputActionValue & Value)
{
	bIsArmed = !bIsArmed;

	if (WeaponManager)
	{
		WeaponManager->ChangeWeapon(2);
	}
}

																												//noise

void AProject_GGFCharacter::GenerateNoise(FVector NoiseLocation, float Intensity, float Radius)
{
	
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