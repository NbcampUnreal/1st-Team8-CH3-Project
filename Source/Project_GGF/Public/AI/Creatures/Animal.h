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
	virtual void OnDie() override;

	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	UDataTable* LootDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FAnimalLoot> LootTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animal")
	EAnimalType AnimalType;

public:
	void GenerateRandomLoot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DrinkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SleepMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	UBoxComponent* AttackHitbox;

	UFUNCTION()
	void OnAttackHitboxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void SetAttackHitboxActive(bool bActive);
};
