#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/Data/AnimalLoot.h"
#include "GGFCharacterBase.generated.h"


UCLASS()
class PROJECT_GGF_API AGGFCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGGFCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddLootToInventory(const TArray<FAnimalLoot>& LootItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FAnimalLoot> GetInventoryLoot() const; 

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FAnimalLoot> Inventory;
};
