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
	FirstPersonCamera->SetActive(false); // 기본적으로 비활성화
	FirstPersonCamera->SetFieldOfView(90.0f);

	// 메시 설정
	ThirdPersonMesh = GetMesh();  // 기본 캐릭터 메시
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetOwnerNoSee(true);



	WeaponSocket = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSocket"));
	WeaponSocket->SetupAttachment(GetMesh(), FName("hand_r")); // 손에 해당하는 Bone에 붙임
	WeaponSocket->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // 상대적인 위치 설정 (필요에 따라 조정)

	CurrentWeapon = nullptr;

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
	//camera
	
	DefaultFOV = 90.0f;  // 기본 시점
	AimFOV = 50.0f;      // 조준 시점
	ZoomInterpSpeed = 10.0f;
	CurrentFOV = 90.0f;   // 기본 FOV
	MinFOV = 45.0f;       // 최대 줌 (4배율)
	MaxFOV = 90.0f;       // 최소 줌 (2배율)
	

}


void AProject_GGFCharacter::BeginPlay()
{
	Super::BeginPlay();

<<<<<<< HEAD


=======
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
	
	WeaponManager = Cast<UWeaponManager>(WeaponManagerPtr.GetDefaultObject());

	if (WeaponManager)
		WeaponManager->CreateWeapons(this);
>>>>>>> origin/feature-items
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

		EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::ZoomIn);

		EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::ZoomOut);

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

		// 1인칭 모드 활성화
		FirstPersonCamera->SetActive(true);
		FollowCamera->SetActive(false);
		SpringArmComp->SetRelativeLocation(FVector(0, 0, 60));  // 1인칭 카메라 위치

		// 메시 조정
		ThirdPersonMesh->SetOwnerNoSee(true);  // 3인칭 메시 숨김
		FirstPersonMesh->SetOwnerNoSee(false); // 1인칭 메시 보이기
	}
	else
	{
		CurrentCameraMode = ECameraMode::ThirdPerson;

		// 3인칭 모드 활성화
		FirstPersonCamera->SetActive(false);
		FollowCamera->SetActive(true);
		SpringArmComp->SetRelativeLocation(FVector(0, 0, 0));  // 3인칭 카메라 위치 초기화

		// 메시 조정
		ThirdPersonMesh->SetOwnerNoSee(false); // 3인칭 메시 보이기
		FirstPersonMesh->SetOwnerNoSee(true);  // 1인칭 메시 숨김
	}
}


void AProject_GGFCharacter::ZoomIn(const FInputActionValue& Value)
{
	if (CurrentCameraMode == ECameraMode::FirstPerson)
	{
		FirstPersonCamera->SetFieldOfView(MinFOV);
	}
}

void AProject_GGFCharacter::ZoomOut(const FInputActionValue& Value)
{
	if (CurrentCameraMode == ECameraMode::FirstPerson)
	{
		FirstPersonCamera->SetFieldOfView(MinFOV);
	}
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