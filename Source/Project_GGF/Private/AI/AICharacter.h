#pragma once

#include "CoreMinimal.h"
#include "AI/GGFAICharacterBase.h"
#include "Items/Manager/WeaponManager.h"
#include "AICharacter.generated.h"

UCLASS()
class PROJECT_GGF_API AAICharacter : public AGGFAICharacterBase
{
	GENERATED_BODY()

public:
	AAICharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UWeaponManager> WeaponManagerPtr;

	UWeaponManager* WeaponManager;

	TArray<FName> HandSockets;
	TArray<FName> BackSockets;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetHandSockets() const { return HandSockets; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetBackSockets() const { return BackSockets; }

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireMontage;
};
