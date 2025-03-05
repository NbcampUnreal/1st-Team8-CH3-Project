#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Interact/Actor/HidePlace.h"
#include "Project_GGF/Public/Interact/GGFInteractiveActor.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" 
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Gameplay/Quest/QuestManager.h"
#include "Project_GGF/Public/Controller/CharacterController.h"
#include "Items/Inventory/InventoryObject.h"
#include "Interact/TreasureChestInteractiveActor.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AProject_GGFCharacter::AProject_GGFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(22.0f, 96.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	CharacterMesh = GetMesh();
	CharacterMesh->SetupAttachment(RootComponent);
	CharacterMesh->SetVisibility(true);
	
	SpringArmComp->SetupAttachment(CharacterMesh);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(25.0f, 68.0f, 71.3f);
	
	FollowCamera->SetupAttachment(SpringArmComp);
	FollowCamera->SetFieldOfView(90.0f);
	FollowCamera->SetRelativeLocation(FVector(100.0f, -20.0f, 10.0f));
	
	CurrentWeapon = nullptr;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	
	//camera
	
	DefaultFOV = 90.0f;  
	AimFOV = 50.0f;      
	ZoomInterpSpeed = 10.0f;
	CurrentFOV = DefaultFOV;
	MinFOV = 22.5f;       
	MaxFOV = 90.0f;     
	ZoomStep = 20.0f;;
	MaxMoveDistance = 250.0f;
}


void AProject_GGFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	WeaponManager = NewObject<UWeaponManager>(this, WeaponManagerPtr);

	if (WeaponManager)
	{
		WeaponManager->CreateWeapons(this);
	}

	InventoryObjectInstance = NewObject<UInventoryObject>(this, InventoryObjectPtr);

	if (InventoryObjectInstance)
	{
		InventoryObjectInstance->CreatePlayerInventory(GetController());
	}

	//ACharacterController* PlayerController = Cast<ACharacterController>(GetController());
	//PlayerController->ShowBackpackInventoryUI();
}

//////////////////////////////////////////////////////////////////////////
// Input
void AProject_GGFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionData.InteractionCheckFrequency)
	{
		if (bIsInteract==true)
		{return;}
		PerformInteractionCheck();
	}

	/*FVector CameraLocation = FollowCamera->GetComponentLocation();
	FRotator CameraRotation = FollowCamera->GetComponentRotation();

	FVector CameraForward = CameraRotation.Vector();

	FVector TraceEnd = CameraLocation + (CameraForward * 10000.0f);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FVector AimPoint = TraceEnd;

	if (Owner->GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		AimPoint = HitResult.ImpactPoint;
	}

	ACharacterController* PlayerController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
	PlayerController->UpdateAimUI(AimPoint);*/
}


void AProject_GGFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		


		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AProject_GGFCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AProject_GGFCharacter::StopJump);

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

		EnhancedInputComponent->BindAction(ThrButtonAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ThirdButtonAction);

		EnhancedInputComponent->BindAction(ForButtonAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::FourthButtonAction);

	
		EnhancedInputComponent->BindAction(UnequipAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::UnequipWeapon);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::Interact);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::UseInventory);
		EnhancedInputComponent->BindAction(MainManuAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::MainManu);
		EnhancedInputComponent->BindAction(ItemUseAction, ETriggerEvent::Triggered, this, &AProject_GGFCharacter::ItemUse);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Move input */
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
		NoiseComp->StopNoiseTimer();
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(NoiseComp->NoiseTimerHandle))
		{
			NoiseComp->StopNoiseTimer();
		}
		NoiseComp->NoiseIntensity = NoiseComp->AverageIntensity;
		NoiseComp->NoiseRadius = NoiseComp->AverageRadiuse;
		NoiseComp->StartNoiseTimer(NoiseComp->NoiseIntensity, NoiseComp->NoiseRadius);
	}
}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
void AProject_GGFCharacter::StartJump(const FInputActionValue& Value)
{
	if (bIsSitting == false)
	{
		Jump();
		bIsfall = true;
	}
	else
	{
		return;
	}
}
void AProject_GGFCharacter::StopJump(const FInputActionValue& Value)
{
	
	StopJumping();
	bIsfall = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Sprint input */
void AProject_GGFCharacter::StartSprint(const FInputActionValue& Value)
{

	if (bIsSitting == true) { ToggleSit(); }
	if (bIsFirstPerson == true) { ToggleZoom(Value); }
	if (bIsAiming == true) { StopAim(); }
///
	if (!StaminaComp || StaminaComp->GetStamina() <= 0 || !GetCharacterMovement())
	{
		StopSprint();
		return;
	}
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}

		if (StaminaComp)
		{
			StaminaComp->StopStaminaRecovery();

			GetWorld()->GetTimerManager().SetTimer(
				SprintStaminaHandle,
				[this]() 
				{
					if (StaminaComp)
					{ StaminaComp->UseStamina();}
				},
				0.5f,
				true
			);
		}
		if (NoiseComp)
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(NoiseComp->NoiseTimerHandle))
			{
				NoiseComp->StopNoiseTimer();
			}
			NoiseComp->StartNoiseTimer(100.0f, 750.0f);
		}
	}
}
//
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Reload input */
void AProject_GGFCharacter::Reload()
{
	if (bIsArmed == true)
	{
		bIsReload = true;
		if (WeaponManager)
		{
			WeaponManager->Reload();
			
			GetWorld()->GetTimerManager().SetTimer(
				ReloadTimer,
				this,
				&AProject_GGFCharacter::FinishReload,
				2.5f,
				false);
		}		
	}
}

void AProject_GGFCharacter::FinishReload()
{
	bIsReload = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Sit input */
void AProject_GGFCharacter::ToggleSit()
{
	if (bIsSitting == false)
	{
		if (bIsSprinting == true)
		{
			StopSprint();
		}
		bIsSitting = true;
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SitSpeed;
			if (bIsInteract == true)
			{
				return;
			}
			if (ZoomState != EZoomState::FirstPerson)
			{
				StartCameraTransition(FVector(100.0f, -20.0f, -50.0f),0.3f);
			}
			else
			{
				FollowCamera->SetRelativeLocation(FVector(280.0f, -48.65f, -49.867f));
				FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
			}
		}
	}
	else
	{
		bIsSitting = false;
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
			if (bIsInteract == true)
			{
				return;
			}
			if (ZoomState != EZoomState::FirstPerson)
			{
				StartCameraTransition(FVector(100.0f, -20.0f, 10.0f),0.3f);
			}
			else
			{
				FollowCamera->SetRelativeLocation(FVector(313.32f, -48.01f, -7.0f));  
				FollowCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 2.98f));
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Aim input */
void AProject_GGFCharacter::StartAim(const FInputActionValue& Value)
{
	if (bIsAiming == true)
	{ return; }
	
	if (ZoomState == EZoomState::FirstPerson)
	{
		return;
	}
	
	bIsAiming = true;

	if (ZoomState == EZoomState::ThirdPerson_Default)
	{
		ZoomState = EZoomState::ThirdPerson_Zoomed;
	}

	TargetFOV = AimFOV;
	GetWorld()->GetTimerManager().SetTimer(
		ZoomTimerHandle,
		this,
		&AProject_GGFCharacter::SetCameraFOV,
		0.01f,
		true);
	
}
void AProject_GGFCharacter::StopAim()
{
	if (bIsAiming == false){ return;}

	bIsAiming = false;
	if (ZoomState == EZoomState::ThirdPerson_Zoomed)
	{
		ZoomState = EZoomState::ThirdPerson_Default;
	}
	TargetFOV = DefaultFOV;
	GetWorld()->GetTimerManager().SetTimer(
		ZoomTimerHandle,
		this,
		&AProject_GGFCharacter::SetCameraFOV,
		0.01f,
		true);
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Zoom input */
void AProject_GGFCharacter::ToggleZoom(const FInputActionValue& Value)
{
	if (bIsArmed== false )
	{
		return;
	}
	else
	{
		if (bIsFirstPerson==false)
		{
			
			ZoomState = EZoomState::FirstPerson;
			SetFirstPersonView();
			bIsFirstPerson = true;
		}
		else  
		{
			ZoomState = EZoomState::ThirdPerson_Default;
			SetThirdPersonView();
			bIsFirstPerson = false;
		}
	}
}


void AProject_GGFCharacter::ZoomScope(const FInputActionValue& Value)
{

	InputValue = Value.Get<float>();

	if (bIsFirstPerson == true || ZoomState == EZoomState::FirstPerson)
	{
		
		CurrentFOV += InputValue * ZoomStep;
		CurrentFOV = FMath::Clamp(CurrentFOV, MinFOV, MaxFOV);
		FollowCamera->SetFieldOfView(CurrentFOV);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Fire input */
void AProject_GGFCharacter::StartFire(const FInputActionValue& Value)
{
	if (bIsSprinting ==true)
	{
		return;
	}
	
	if (bIsArmed == true)
	{
		if (WeaponManager)
		{
			bIsFiring = true;
			WeaponManager->Attack();
		}
		NoiseComp->NoiseIntensity = 200.0f;
		NoiseComp->NoiseRadius = 1500.0f;
		NoiseComp->GenerateNoiseTimer();
	
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AProject_GGFCharacter::StopFire, 0.25f, false);
	}

	if (bIsGranade == true)
	{
		if (WeaponManager)
		{
			bIsThrow = true;
			WeaponManager->Attack();
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AProject_GGFCharacter::StopFire, 0.25f, false);
		}
	}
}

void AProject_GGFCharacter::StopFire()
{
	if (bIsFiring==true)
	{
		bIsFiring = false;
	}
	if (bIsThrow == true)
	{
		bIsThrow = false;
		bIsGranade = false;
	}
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////** Called for Quiet input */
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////**Called for Number Button input */
void AProject_GGFCharacter::FirstButtonAction(const FInputActionValue& Value)
{

	if (bIsArmed == false)
	{
		if (WeaponManager)
		{
			bIsArmed = true;
			WeaponManager->ChangeWeapon(1);
			
		}
	}
	else
	{
		if (WeaponManager)
		{
			bIsArmed = false;
			WeaponManager->ChangeWeapon(0);
		}
	}
}


void AProject_GGFCharacter::SecondButtonAction(const FInputActionValue & Value)
{
	if (bIsArmed == false)
	{
		if (WeaponManager)
		{
			bIsArmed = true;
			WeaponManager->ChangeWeapon(2);
			
		}
	}
	else
	{
		if (WeaponManager)
		{
			bIsArmed = false;
			WeaponManager->ChangeWeapon(0);
			
		}
	}
}
void AProject_GGFCharacter::ThirdButtonAction(const FInputActionValue& Value)
{
	if (bIsArmed == false)
	{
		if (WeaponManager)
		{
			bIsGranade = true;
			WeaponManager->ChangeWeapon(3);
		}
	}
	else
	{
		if (WeaponManager)
		{
			bIsArmed = false;
			WeaponManager->ChangeWeapon(0);
		}
	}
}
void AProject_GGFCharacter::FourthButtonAction(const FInputActionValue& Value)
{
	if (bIsArmed == false)
	{
		if (WeaponManager)
		{
			bIsGranade = true;
			WeaponManager->ChangeWeapon(4);
		}
	}
	else
	{
		if (WeaponManager)
		{
			bIsArmed = false;
			WeaponManager->ChangeWeapon(0);
		}
	}
}

void AProject_GGFCharacter::EndThrow()
{
	bIsGranade = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////**

void AProject_GGFCharacter::Interact(const FInputActionValue& Value)
{
    if (FocusedHidePlace)
    {
        if (LastCheckedInteractActor != FocusedHidePlace)
        {
            if (LastCheckedInteractActor)
            {
                AHidePlace* LastHidePlace = Cast<AHidePlace>(LastCheckedInteractActor);
                if (LastHidePlace)
                {
                    LastHidePlace->ShowInteractionWidget(false);
                }
            }
            LastCheckedInteractActor = FocusedHidePlace;
            FocusedHidePlace->ShowInteractionWidget(true);
        }
    	
        if (bIsInteract)
        {
            if (LastCheckedInteractActor)
            {
                AHidePlace* HidePlace = Cast<AHidePlace>(LastCheckedInteractActor);
                if (HidePlace)
                {
                    HidePlace->ExitShelter(this);
                }
            }
            return;
        }

        bIsInteract = true;
    	
        if (CharacterMesh)
        {
            SetActorEnableCollision(false);
        }
    	
        if (LastCheckedInteractActor)
        {
            UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(LastCheckedInteractActor->GetRootComponent());
            if (PrimComp)
            {
                PrimComp->SetSimulatePhysics(false);
                PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            }
        }
    	
        if (InteractMontage && GetMesh() && GetMesh()->GetAnimInstance())
        {
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(InteractMontage, 1.0f);
                AnimInstance->SetRootMotionMode(ERootMotionMode::NoRootMotionExtraction);
            }
        }
        
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            [this]()
            {
                if (LastCheckedInteractActor)
                {
                    AHidePlace* HidePlace = Cast<AHidePlace>(LastCheckedInteractActor);
                    if (HidePlace)
                    {
                        HidePlace->InteractionKeyPressed(this);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("LastCheckedInteractActor is invalid in timer callback"));
                }
            },
            1.7f,
            false
        );
    }

	if (InteractableActor)
	{
		ATreasureChestInteractiveActor* ChestActor = Cast<ATreasureChestInteractiveActor>(InteractableActor);
		ChestActor->InteractionKeyPressed(this);
	}
}


void AProject_GGFCharacter::EndInteract()
{
	if (LastCheckedInteractActor)
	{
		bIsInteract = false;

		if (CharacterMesh)
		{
			SetActorEnableCollision(true);
		}
		
		AHidePlace* HidePlace = Cast<AHidePlace>(LastCheckedInteractActor);
		if (HidePlace)
		{
			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HidePlace->GetRootComponent());
			if (PrimComp)
			{
				PrimComp->SetSimulatePhysics(true);
				PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
			}
		}
		
		FTimerHandle EndInteractTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			EndInteractTimerHandle,
			[this]()
			{
				if (LastCheckedInteractActor)
				{
					AHidePlace* HidePlace = Cast<AHidePlace>(LastCheckedInteractActor);
					if (HidePlace)
					{
						HidePlace->ShowInteractionWidget(false);
					}
				}

				LastCheckedInteractActor = nullptr;
			},
			1.0f,
			false
		);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AProject_GGFCharacter::UnequipWeapon(const FInputActionValue& Value)
{
	if (bIsArmed == true)
	{
		if (WeaponManager)
		{
			bIsArmed = false;
			WeaponManager->ChangeWeapon(0);
		}
	}
	else
	{
		return;
	}
	if (bIsGranade == true)
	{
		if (WeaponManager)
		{
			bIsGranade = false;
			WeaponManager->ChangeWeapon(0);
		}
	}
	else
	{
		return;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AProject_GGFCharacter::UseInventory(const FInputActionValue& Value)
{
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void AProject_GGFCharacter::MainManu(const FInputActionValue& Value)
{
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void AProject_GGFCharacter::ItemUse(const FInputActionValue& Value)
{
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
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

void AProject_GGFCharacter::SetThirdPersonView()
{
	CharacterMesh->UnHideBoneByName(FName("neck_01"));
	FollowCamera->SetRelativeLocation(FVector(100.0f, -20.0f, 10.0f));
	if (bIsSitting == true)
	{
		FollowCamera->SetRelativeLocation(FVector(100.0f, -20.0f, -50.0f));
	}
	FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
	FollowCamera->SetFieldOfView(90.0f);
}

void AProject_GGFCharacter::SetFirstPersonView()
{
	CharacterMesh->HideBoneByName(FName("neck_01"), PBO_None);
	FollowCamera->SetRelativeLocation(FVector(313.32f, -48.01f, -1.0f));  
	FollowCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 2.98f));
	if (bIsSitting == true)
	{
		FollowCamera->SetRelativeLocation(FVector(280.0f, -48.65f, -45.867f));  
		FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
	}
	FollowCamera->SetFieldOfView(120.0f);

	FollowCamera->bUsePawnControlRotation = true;
}

void AProject_GGFCharacter::StartCameraTransition(FVector NewLocation, float Duration)
{
	StartLocation = FollowCamera->GetRelativeLocation();
	TargetLocation = NewLocation;
	ElapsedTime = 0.0f;
	TransitionDuration = Duration; 

	GetWorld()->GetTimerManager().SetTimer(CameraMoveTimer, this, &AProject_GGFCharacter::UpdateCameraPosition, 0.01f, true);
}

void AProject_GGFCharacter::UpdateCameraPosition()
{
	if (TransitionDuration <= 0.0f) 
	{
		FollowCamera->SetRelativeLocation(TargetLocation);
		GetWorld()->GetTimerManager().ClearTimer(CameraMoveTimer);
		return;
	}

	ElapsedTime += 0.01f;
	float Alpha = ElapsedTime / TransitionDuration; 
    
	FVector NewPosition = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	FollowCamera->SetRelativeLocation(NewPosition);

	if (Alpha >= 1.0f)  
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraMoveTimer);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AProject_GGFCharacter::AddItemToInventory(FString ItemName, int32 Amount)
{
	if (QuestManager)
	{
		QuestManager->UpdateQuestProgress(ItemName, Amount);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AProject_GGFCharacter::SetNearbyInteractiveObject(AGGFInteractiveActor* InteractiveObject)
{
	NearbyInteractiveObject = InteractiveObject;
    
	if (NearbyInteractiveObject.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("NearbyInteractiveObject set to: %s"), *NearbyInteractiveObject->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("NearbyInteractiveObject cleared"));
	}
}


void AProject_GGFCharacter::PerformInteractionTrace()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = ((ForwardVector * 500.0f) + Start);  

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		if (HitResult.GetActor() && HitResult.GetActor()->IsA(AHidePlace::StaticClass()))
		{
			
			if (!FocusedHidePlace || (GetWorld()->GetTimeSeconds() - LastInteractionTime) > InteractionCooldownTime)
			{
				FocusedHidePlace = Cast<AHidePlace>(HitResult.GetActor());
				FocusedHidePlace->ShowInteractionWidget(true);
			}
		}
		else
		{
			if (FocusedHidePlace)
			{
				FocusedHidePlace->ShowInteractionWidget(false);
			}
			FocusedHidePlace = nullptr;
		}
	}
	else
	{
		if (FocusedHidePlace)
		{
			FocusedHidePlace->ShowInteractionWidget(false);
		}
		FocusedHidePlace = nullptr;
	}
}


void AProject_GGFCharacter::PerformInteractionCheck()
{
	PerformInteractionTrace();
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
}

