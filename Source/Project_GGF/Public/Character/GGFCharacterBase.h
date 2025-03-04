#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Project_GGF\Public\Character\GGFCharacterBase.h"
#include "Project_GGF\Public\Character\Data/StaminaComponent.h"
#include "Project_GGF\Public\Character\Data/NoiseComponent.h"
#include "Logging/LogMacros.h"
#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Camera/CameraComponent.h"
#include "Items/UtiliyItem/ThrowingItem.h"
#include "GameFramework/Character.h"
#include "Interact/Actor/HidePlace.h"
#include "Items/Data/AnimalLoot.h"
#include "GGFCharacterBase.generated.h"

class USpringArmComponent;

struct FInputActionValue;
struct FInteractionData
{
	float LastInteractionCheckTime = 0.0f;  
	float InteractionCheckFrequency = 1.0f; 
};

UCLASS()
class PROJECT_GGF_API AGGFCharacterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AGGFCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:	
	virtual void Tick(float DeltaTime) override;
	//////////////////////////////////////////컴포넌트들
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHitDeadComponent* HitDeadComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNoiseComponent* NoiseComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class URespawnComponent* RespawnComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	AActor* InteractableActor;
	AHidePlace* FocusedHidePlace;
	FInteractionData InteractionData;
	////////////////////////////////////////// 입력 함수
	
	UFUNCTION()
	virtual void StartSprint();
	UFUNCTION()
	virtual void UpdateSprint();
	UFUNCTION()
	virtual void StopSprint();
	
	UFUNCTION()
	virtual void Reload();
	UFUNCTION()
	virtual void UpdateReload();
	UFUNCTION()
	virtual void FinishReload();
	
	UFUNCTION()
	virtual void ToggleSit();

	UFUNCTION()
	virtual void StartFire();
	UFUNCTION()
	virtual void UpdateFire();
	UFUNCTION()
	virtual void StopFire();
	
	UFUNCTION()
	virtual void StartQuiet();
	UFUNCTION()
	virtual void UpdateQuiet();
	UFUNCTION()
	virtual void StopQuiet();

	UFUNCTION()
	virtual void Interact();
	UFUNCTION()
	virtual void UpdateInteract();
	UFUNCTION()
	virtual void EndInteract();
	
	////////////////////////////////////////// 타이
	FTimerHandle SprintStaminaHandle;
	FTimerHandle SpeedBoostTimerHandle;
	FTimerHandle ReloadTimer;
	FTimerHandle ThrowTimerHandle;
	FTimerHandle FireTimerHandle;
	////////////////////////////////////////// 소켓
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	FName HandSocket_Left = "L_HandSocket";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	FName HandSocket_Right = "R_HandSocket";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	FName BackSocket_Left = "L_BackSocket";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	FName BackSocket_Right = "R_BackSocket";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	FName LeftHand ="hand_l";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	TArray<FName> HandSockets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Socket")
	TArray<FName> BackSockets;


	////////////////////////////////////////// 무기 매니저 및 무기 관련

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TObjectPtr<AThrowingItem> CurrentThrowableItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UWeaponManager> WeaponManagerPtr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UWeaponManager* WeaponManager;


	////////////////////////////////////////// 기본 스테이터스 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float SprintSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float SpeedBoostDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float SpeedBoostMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float SitSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float SitSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float QuietSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float QuietSpeed;

	float InteractionCooldownTime = 4.0f; 
	float LastInteractionTime = 0.0f;

	
	int32 InteractNumber;
	
	///////////////////////////////////////////// BP 관련 Bool
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sit")
	bool bIsSitting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiet")
	bool bIsQuiet = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FirstPerson")
	bool bIsFirstPerson = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	bool bIsAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload")
	bool bIsReload = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armed")
	bool bIsArmed = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
	bool bIsFiring = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	bool bIsInteract = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UesItem")
	bool bIsUseItem = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UesItem")
	bool bIsfall = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UesItem")
	bool bIsGranade = false;
	///////////////////////////////////////////// 레이캐스트
	
	virtual void PerformInteractionTrace();
	virtual void PerformInteractionCheck();
	
	////////////////////////////////////////////
	
	virtual void ActivateSpeedBoost();
	virtual void ResetSpeedBoost();
	
	///////////////////////////////////////////// 인벤토리
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddLootToInventory(const TArray<FAnimalLoot>& LootItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FAnimalLoot> GetInventoryLoot() const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FName GetHandLSockets() const { return HandSocket_Left; }
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FName GetHandRSockets() const { return HandSocket_Right; }
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetBackSockets() const { return BackSockets; }
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetHandSockets() const { return HandSockets; }
	FName GetLeftHand() const { return LeftHand; }

	///////////////////////////////////////////// AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FGenericTeamId TeamId;
	
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	UFUNCTION(BlueprintCallable)
	virtual void OnDie();

	UFUNCTION(BlueprintCallable)
	virtual void OnHit(AActor* Attacker);

private:
	bool IsDead = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FAnimalLoot> Inventory;
};

