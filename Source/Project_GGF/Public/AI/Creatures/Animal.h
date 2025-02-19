#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/Quest/QuestItemData.h"
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
class PROJECT_GGF_API AAnimal : public ACharacter
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DrinkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SleepMontage;

private:
	void UpdateTimeState();
	void UpdateAttackState(bool bIsHit);

	class AGGFGameMode* GameMode;
	class UBlackboardComponent* BlackboardComponent;
};
