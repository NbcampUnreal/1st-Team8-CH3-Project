#pragma once

#include "CoreMinimal.h"
#include "Project_GGF\Public\Character\GGFCharacterBase.h"
#include "Logging/LogMacros.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Interact/GGFInteractiveActor.h"
#include "Interact/Actor/HidePlace.h"
#include "InputAction.h"
#include "Project_GGFCharacter.generated.h"


struct FInputActionValue;


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

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USkeletalMeshComponent* CharacterMesh;
	
	EZoomState ZoomState = EZoomState::ThirdPerson_Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* QuietAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomScopeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SitAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MainManuAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ItemUseAction;
	
	
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

	//Aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float UpperBodyYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float UpperBodyPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float TargetYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Offset")
	float TargetPitch;
	
	FVector StartLocation;
	FVector TargetLocation;
	float ElapsedTime;
	float TransitionDuration;
	///////////////////////////////////////Weapon

	TWeakObjectPtr<AGGFInteractiveActor> NearbyInteractiveObject;
	void SetNearbyInteractiveObject(AGGFInteractiveActor* InteractiveObject);

	float MaxMoveDistance;
	//////////////////////////
	UPROPERTY()
	AActor* LastCheckedInteractActor;
	///
	FTimerHandle SprintStaminaHandle;
	FTimerHandle SpeedBoostTimerHandle;
	FTimerHandle ReloadTimer;



	FTimerHandle ZoomTimerHandle;
	FTimerHandle ThrowTimerHandle;
	FTimerHandle FireTimerHandle;
	FTimerHandle CameraMoveTimer;
	
public:
	AProject_GGFCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
	AGGFInteractiveActor* FocusedActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* InteractMontage;
	
	void Move(const FInputActionValue& Value);

	
	void Look(const FInputActionValue& Value);
	
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	
	
	void StartSprint(const FInputActionValue& Value);
	void StopSprint() override;

	
	void Reload();
	void FinishReload() override;
			
	
	void ToggleSit();


	void StartAim(const FInputActionValue& Value);
	void StopAim();

	
	void ToggleZoom(const FInputActionValue& Value);
	void ZoomScope(const FInputActionValue& Value);

	
	void StartFire(const FInputActionValue& Value);
	void StopFire() override;
	
	void StartQuiet(const FInputActionValue& Value);
	void StopQuiet() override;
	
	void FirstButtonAction(const FInputActionValue& Value);
	void SecondButtonAction(const FInputActionValue& Value);
	void ThirdButtonAction(const FInputActionValue& Value);
	void FourthButtonAction(const FInputActionValue& Value);
	void EndThrow();

	void Interact(const FInputActionValue& Value);
	void EndInteract() override;
	
	void UnequipWeapon(const FInputActionValue& Value);

	void UseInventory(const FInputActionValue& Value);
	void MainManu(const FInputActionValue& Value);
	void ItemUse(const FInputActionValue& Value);
	
protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	
	//Camera
	void SetCameraFOV();
	void SetThirdPersonView();
	void SetFirstPersonView();
	void StartCameraTransition(FVector NewLocation, float Duration);
	void UpdateCameraPosition();
	

	// Weapon
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(FString ItemName, int32 Amount);
	
	void PerformInteractionCheck();
	void PerformInteractionTrace();
	
	
	AHidePlace* FocusedHidePlace;
private:
	UPROPERTY()
	class AQuestManager* QuestManager;
};

