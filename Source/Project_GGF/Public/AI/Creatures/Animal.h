#pragma once

#include "CoreMinimal.h"
#include "AI/GGFAICharacterBase.h"
#include "Items/Data/AnimalLoot.h"
#include "Animal.generated.h"


UCLASS()
class PROJECT_GGF_API AAnimal : public AGGFAICharacterBase
{
	GENERATED_BODY()

public:
	AAnimal();

	UFUNCTION(BlueprintCallable)
	TArray<FAnimalLoot> GetLoot() const { return LootTable; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	UDataTable* LootDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FAnimalLoot> LootTable;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animal")
	EAnimalType AnimalType;

public:
	void GenerateRandomLoot();
	void UpdateAttackState(bool bIsHit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DrinkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SleepMontage;

private:
	void ResetAttackState();

	class UBlackboardComponent* BlackboardComponent;
	FTimerHandle AttackResetTimerHandle;
};
