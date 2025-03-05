#pragma once

#include "CoreMinimal.h"
#include "AI/GGFAICharacterBase.h"
#include "GGFAICharacter.generated.h"

UCLASS()
class PROJECT_GGF_API AGGFAICharacter : public AGGFAICharacterBase
{
	GENERATED_BODY()
	
public:
	AGGFAICharacter();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void Aiming();
	
	UFUNCTION(BlueprintCallable)
	void Shoot();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireMontage;

private:
	FVector LootLocation;  // 전리품 위치

public:
	void SetLootLocation(FVector NewLocation);
	FVector GetLootLocation() const { return LootLocation; };
	void ClearLootLocation() { LootLocation = FVector::ZeroVector; }

};
