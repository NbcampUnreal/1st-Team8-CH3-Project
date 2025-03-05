// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h" 
#include "InputMappingContext.h"
#include "CharacterController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInventory;

UCLASS()
class PROJECT_GGF_API ACharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACharacterController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	// IA_Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	// IA_Jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	// IA_Look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	// IA_Quiet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* QuietAction;
	// IA_Aim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AimAction;
	// IA_Fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;
	// IA_Sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	// IA_Reload
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ReloadAction;
	// IA_Sit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SitAction;
	// IA_Zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ZoomAction;
	// IA_ZoomOut
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ZoomScopeAction;
	// IA_1_button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FirButtonAction;
	// IA_2_button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SecButtonAction;
	// IA_3_button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ThirdButtonAction;
	// IA_4_button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FourthButtonAction;
	// IA_Interact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;
	// IA_Unequip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UnequipAction;

public:
	void ShowBackpackInventoryUI();
	void RemoveBackpackInventoryUI();
	void ShowInteractInventoryUI(UUserWidget* Widget);
	void RemoveInteractInventoryUI(UUserWidget* Widget);

	virtual void BeginPlay() override;
	
};
