// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Component/HealthComponent.h"
#include "Project_GGF/Public/Component/StaminaComponent.h"
#include "Project_GGF/Public/Component/RespawnComponent.h"
#include "Project_GGF/Public/Component/NoiseComponent.h"
#include "Project_GGFCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USceneComponent;
struct FInputActionValue;


UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	ThirdPerson,
	FirstPerson
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProject_GGFCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// �⺻ ĳ���� ������Ʈ //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* ThirdPersonMesh;

	// ī�޶� ������Ʈ //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FirstPersonCamera;

	ECameraMode CurrentCameraMode = ECameraMode::ThirdPerson;



	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	/** Quiet Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* QuietAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AimAction;

	/** Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomScopeAction;


	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	/** Sit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SitAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

	/** button Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FirButtonAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SecButtonAction;

	//���� ����//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* WeaponSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;

	// Sprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	bool bIsSprinting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SprintSpeed")
	float SprintSpeedMultiplier;
	float SprintSpeed;

	//Sit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	bool bIsSitting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SitSpeed")
	float SitSpeedMultiplier;
	float SitSpeed;

	//Quiet
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	bool bIsQuiet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuietSpeed")
	float QuietSpeedMultiplier;
	float QuietSpeed;


	//Zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float DefaultFOV;  // �⺻ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float AimFOV;      // ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomInterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float CurrentFOV;   // �⺻ FOV
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MinFOV;      // �ִ� �� (4����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MaxFOV;      // �ּ� �� (2����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float InputValue;
	float ZoomStep;
	float TargetFOV;

	//Aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float UpperBodyYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float UpperBodyPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float TargetYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float TargetPitch;


	//Armed
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsArmed = true;


	//Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponManager")
	//UWeaponManager* WeaponManager;
	//TSoftClassPtr<UWeaponManager> WeaponManagerPtr;
	TSubclassOf<UWeaponManager> WeaponManagerPtr;

	UWeaponManager* WeaponManager;
	//////////////////////////////////Componenst
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URespawnComponent* RespawnComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNoiseComponent* NoiseComp;
	/// /////////////////////////////////////////
	
	FTimerHandle StaminaRestoreHandle;
	FTimerHandle TimerHandle_Respawn;
	FTimerHandle SprintStaminaHandle;
	FTimerHandle ZoomTimerHandle;
	

public:
	AProject_GGFCharacter();

	virtual void BeginPlay() override;


protected:

	/** Called for movement input */
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	/** Called for Sprint input */
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);

	/** Called for Reload input */
	UFUNCTION()
	void Reload(const FInputActionValue& Value);
			
	/** Called for Sit input */
	UFUNCTION()
	void ToggleSit(const FInputActionValue& Value);

	/** Called for Aim input */
	UFUNCTION()
	void StartAim(const FInputActionValue& Value);
	UFUNCTION()
	void StopAim(const FInputActionValue& Value);

	/** Called for Zoom input */
	UFUNCTION()
	void ToggleZoom(const FInputActionValue& Value);

	UFUNCTION()
	void ZoomScope(const FInputActionValue& Value);

	/** Called for Fire input */
	UFUNCTION()
	void StartFire(const FInputActionValue& Value);
	UFUNCTION()
	void StopFire(const FInputActionValue& Value);

	/** Called for Quiet input */
	UFUNCTION()
	void StartQuiet(const FInputActionValue& Value);
	UFUNCTION()
	void StopQuiet(const FInputActionValue& Value);

	/** Called for Button input */
	UFUNCTION()
	void FirstButtonAction(const FInputActionValue& Value);
	UFUNCTION()
	void SecondButtonAction(const FInputActionValue& Value);



protected:


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:


	//camera
	void SetCameraFOV();

	USceneComponent* GetWeaponSocket() { return WeaponSocket; }
};

