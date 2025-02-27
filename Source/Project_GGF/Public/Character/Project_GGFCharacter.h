// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/GGFCharacterBase.h"
#include "Logging/LogMacros.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Character/Data/HealthComponent.h"
#include "Project_GGF/Public/Interact/GGFInteractiveActor.h"
#include "Project_GGF/Public/Character/Data/StaminaComponent.h"
#include "Project_GGF/Public/Character/Data/RespawnComponent.h"
#include "Project_GGF/Public/Character/Data/NoiseComponent.h"
#include "Interact/Actor/HidePlace.h"
#include "Items/UtiliyItem/ThrowingItem.h"
#include "Project_GGFCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USceneComponent;
struct FInputActionValue;

struct FInteractionData
{
	float LastInteractionCheckTime = 0.0f;  
	float InteractionCheckFrequency = 1.0f; 
};

UENUM(BlueprintType)
enum class EZoomState : uint8
{
	ThirdPerson_Default UMETA(DisplayName = "Third Person Default"),
	ThirdPerson_Zoomed UMETA(DisplayName = "Third Person Zoomed"),
	FirstPerson UMETA(DisplayName = "First Person")
};



DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProject_GGFCharacter : public AGGFCharacterBase
{
	GENERATED_BODY()

public:

	FInteractionData InteractionData;
	
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	AActor* InteractableActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* CharacterMesh;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	EZoomState ZoomState = EZoomState::ThirdPerson_Default;

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
	UInputAction* ThrButtonAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ForButtonAction;

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
	float DefaultFOV;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float AimFOV;      
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomInterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float CurrentFOV;   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MinFOV;      
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MaxFOV;      
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

	UPROPERTY(BlueprintReadOnly, Category = "Fire")
	bool bIsFiring = false;


	//interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	bool bIsInteract = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Use")
	bool bIsUseItem = false;

	

	///////////////////////////////////////Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponManager")
	TSubclassOf<UWeaponManager> WeaponManagerPtr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName ThrowSocket = "ThrowSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName HandSocket_Left = "L_HandSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName HandSocket_Right = "R_HandSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName BackSocket_Left = "L_BackSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName BackSocket_Right = "R_BackSocket";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName LeftHand ="hand_l";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<FName> HandSockets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<FName> BackSockets;

	UWeaponManager* WeaponManager;
	//////////////////////////////////Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URespawnComponent* RespawnComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNoiseComponent* NoiseComp;
	/// /////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TObjectPtr<AThrowingItem> CurrentThrowableItem;
	

	/// /////////////////////////////////////////

	TWeakObjectPtr<AGGFInteractiveActor> NearbyInteractiveObject;
	
	void SetNearbyInteractiveObject(AGGFInteractiveActor* InteractiveObject);
	
	
	/// /////////////////////////////////////////

	FTimerHandle StaminaRestoreHandle;
	FTimerHandle TimerHandle_Respawn;
	FTimerHandle SprintStaminaHandle;
	FTimerHandle ZoomTimerHandle;
	FTimerHandle SpeedBoostTimerHandle;
	FTimerHandle ReloadTimer;
	FTimerHandle ThrowTimerHandle;
	FTimerHandle FireTimerHandle;

public:
	AProject_GGFCharacter();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	void PerformInteractionTrace();
	AGGFInteractiveActor* FocusedActor = nullptr;


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
	void StopFire();

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
	UFUNCTION()
	void ThirdButtonAction(const FInputActionValue& Value);
	UFUNCTION()
	void FourthButtonAction(const FInputActionValue& Value);
	
	/** Called for Interact input */
	UFUNCTION()
	void Interact(const FInputActionValue& Value);
	void EndInteract();

	void UnequipWeapon(const FInputActionValue& Value);

protected:


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	void ActivateSpeedBoost();
	void ResetSpeedBoost();
	//////////////////////////////////////


	//Camera
	void SetCameraFOV();
	void SetThirdPersonView();
	void SetFirstPersonView();
	

	// Weapon
	
	FName GetHandLSockets() const { return HandSocket_Left; }
	FName GetHandRSockets() const { return HandSocket_Right; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetBackSockets() const { return BackSockets; }
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetHandSockets() const { return HandSockets; }
	FName GetLeftHand() const { return LeftHand; }
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(FString ItemName, int32 Amount);


	
	float InteractionCooldownTime = 4.0f; 
	float LastInteractionTime = 0.0f;
	void PerformInteractionCheck();
	AHidePlace* FocusedHidePlace;
private:
	UPROPERTY()
	class AQuestManager* QuestManager;
};

