// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Character/Data/HealthComponent.h"
#include "Project_GGF/Public/Character/Data/StaminaComponent.h"
#include "Project_GGF/Public/Character/Data/RespawnComponent.h"
#include "Project_GGF/Public/Character/Data/NoiseComponent.h"
#include "Project_GGFCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USceneComponent;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProject_GGFCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// �⺻ ĳ���� ������Ʈ //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* CharacterMesh;

	// ī�޶� ������Ʈ //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UnequipAction;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;
	
	
	
	// Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpeedBoost")
	float SpeedBoostDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpeedBoost")
	float SpeedBoostMultiplier;
	float MaxSpeed;
	
	// Sprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	float SprintSpeedMultiplier;
	float SprintSpeed;

	//Sit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit")
	bool bIsSitting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit")
	float SitSpeedMultiplier;
	float SitSpeed;

	//Quiet
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiet")
	bool bIsQuiet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiet")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	bool bIsFirstPerson = false;

	//Aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float UpperBodyYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float UpperBodyPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float TargetYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float TargetPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	bool bIsAiming = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload")
	bool bIsReload = false;

	//Armed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armed")
	bool bIsArmed = false;


	///Throwing
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwing")
	float ThrowStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwing")
	bool EquippedThrowableItem = false;

	///////////////////////////////////////Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponManager")
	TSubclassOf<UWeaponManager> WeaponManagerPtr;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName HandSocket_Left = "L_HandSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName HandSocket_Right = "R_HandSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName BackSocket_Left = "L_BackSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName BackSocket_Right = "R_BackSocket";

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<FName> HandSockets;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<FName> BackSockets;

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
	FTimerHandle SpeedBoostTimerHandle;
	FTimerHandle ReloadTimer;

	AProject_GGFCharacter();
	virtual void BeginPlay() override;




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
	void StopSprint();

	/** Called for Reload input */
	UFUNCTION()
	void Reload(const FInputActionValue& Value);
	void FinishReload();
			
	/** Called for Sit input */
	UFUNCTION()
	void ToggleSit(const FInputActionValue& Value);

	/** Called for Aim input */
	UFUNCTION()
	void StartAim(const FInputActionValue& Value);
	UFUNCTION()
	void StopAim();

	/** Called for Zoom input */
	UFUNCTION()
	void ToggleZoom(const FInputActionValue& Value);

	UFUNCTION()
	void ZoomScope(const FInputActionValue& Value);

	/** Called for Fire input */
	UFUNCTION()
	void StartFire(const FInputActionValue& Value);

	/** Called for Quiet input */
	UFUNCTION()
	void StartQuiet(const FInputActionValue& Value);
	UFUNCTION()
	void StopQuiet();

	/** Called for Button input */
	UFUNCTION()
	void FirstButtonAction(const FInputActionValue& Value);
	UFUNCTION()
	void SecondButtonAction(const FInputActionValue& Value);


	/** Called for Interact input */
	UFUNCTION()
	void Interact(const FInputActionValue& Value);

	void UnequipWeapon(const FInputActionValue& Value);

protected:


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	void ActivateSpeedBoost();
	void ResetSpeedBoost();
	

	//Camera
	void SetCameraFOV();


	// Weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetHandSockets() const { return HandSockets; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetBackSockets() const { return BackSockets; }


	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(FString ItemName, int32 Amount);

private:
	UPROPERTY()
	class AQuestManager* QuestManager;
};

