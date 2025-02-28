#pragma once

#include "CoreMinimal.h"
#include "Project_GGF\Public\Character\GGFCharacterBase.h"
#include "Logging/LogMacros.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Interact/GGFInteractiveActor.h"
#include "Interact/Actor/HidePlace.h"
#include "InputAction.h"
#include "Items/Inventory/InventoryObject.h"
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
	

	///////////////////////////////////////Weapon

	TWeakObjectPtr<AGGFInteractiveActor> NearbyInteractiveObject;
	void SetNearbyInteractiveObject(AGGFInteractiveActor* InteractiveObject);
	
	FTimerHandle SprintStaminaHandle;
	FTimerHandle SpeedBoostTimerHandle;
	FTimerHandle ReloadTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventoryObject> InventoryObjectPtr;
	UInventoryObject* InventoryObjectInstance;

	FTimerHandle ZoomTimerHandle;
	FTimerHandle ThrowTimerHandle;
	FTimerHandle FireTimerHandle;
	
public:
	AProject_GGFCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
	AGGFInteractiveActor* FocusedActor = nullptr;


	
	void Move(const FInputActionValue& Value);

	
	void Look(const FInputActionValue& Value);

	
	void StartSprint(const FInputActionValue& Value);
	void StopSprint() override;

	
	void Reload(const FInputActionValue& Value);
	void FinishReload() override;
			
	
	void ToggleSit(const FInputActionValue& Value);


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
	

	void Interact(const FInputActionValue& Value);
	void EndInteract() override;
	
	void UnequipWeapon(const FInputActionValue& Value);

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	
	//Camera
	void SetCameraFOV();
	void SetThirdPersonView();
	void SetFirstPersonView();
	

	// Weapon
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(FString ItemName, int32 Amount);

	UInventoryObject* GetInventoryObject() { return InventoryObjectInstance; }
	
	void PerformInteractionCheck();
	void PerformInteractionTrace();
	
	AHidePlace* FocusedHidePlace;
private:
	UPROPERTY()
	class AQuestManager* QuestManager;
};

