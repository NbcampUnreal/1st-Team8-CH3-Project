#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/Manager/WeaponManager.h"
#include "AICharacter.generated.h"

UCLASS()
class PROJECT_GGF_API AAICharacter : public ACharacter
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* WeaponSocket;

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireMontage;

	USceneComponent* GetWeaponSocket()
	{
		if (WeaponSocket == nullptr)
		{
			return nullptr;
		}

		return WeaponSocket;
	}
};
