#pragma once

#include "CoreMinimal.h"
#include "AI/GGFAICharacterBase.h"
#include "Gameplay/Quest/QuestItemData.h"
#include "Character/Data/HealthComponent.h"
#include "Animal.generated.h"

USTRUCT(BlueprintType)
struct FAnimalLoot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	FQuestItemData ItemData; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int32 Quantity; 
};

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
	virtual void OnDeath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FAnimalLoot> LootTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent;

public:
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
