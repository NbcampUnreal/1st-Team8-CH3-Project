// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Component/HealthComponent.h"
#include "Project_GGFCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USceneComponent;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FNoise
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;  // Noise �߻� ��ġ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity;  // Noise�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;  // Noise�� ������ ��ġ�� ����

	FNoise() : Location(FVector::ZeroVector), Intensity(0.f), Radius(500.f) 
	{
	}
};

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

	// ī�޶� ������Ʈ //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FirstPersonCamera;

	ECameraMode CurrentCameraMode = ECameraMode::ThirdPerson;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* ThirdPersonMesh;



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
	UInputAction* ZoomInAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomOutAction;


	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	/** Sit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SitAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

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

	//Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

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
	float ZoomStep;
	float TargetFOV;


	//Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponManager")
	//UWeaponManager* WeaponManager;
	//TSoftClassPtr<UWeaponManager> WeaponManagerPtr;
	TSubclassOf<UWeaponManager> WeaponManagerPtr;

	UWeaponManager* WeaponManager;

	UPROPERTY()
	UHealthComponent* HealthComp;


	FTimerHandle StaminaRestoreHandle;
	FTimerHandle TimerHandle_Respawn;
	FTimerHandle SprintStaminaHandle;
	FTimerHandle NoiseTimerHandle;
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
	void ZoomIn(const FInputActionValue& Value);

	UFUNCTION()
	void ZoomOut(const FInputActionValue& Value);

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


protected:


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:


	// Stamina
	void RestoreStamina();
	void UseStamina();
	void StartStaminaRecovery();
	void StopStaminaRecovery();

	//Noise
	void GenerateNoise(FVector NoiseLocation, float Intensity, float Radius);
	void GenerateNoiseTimer(float Intensity, float Radius);
	void StopNoiseTimer();

	//camera
	void SetCameraFOV();
};

