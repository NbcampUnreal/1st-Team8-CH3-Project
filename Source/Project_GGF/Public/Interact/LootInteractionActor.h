#pragma once

#include "CoreMinimal.h"
#include "Interact/GGFInteractiveActor.h"
#include "Items/Data/AnimalLoot.h"
#include "LootInteractionActor.generated.h"

class UInventoryObject;

UCLASS()
class PROJECT_GGF_API ALootInteractionActor : public AGGFInteractiveActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	// 전리품 데이터 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<FAnimalLoot> LootItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventoryObject> InventoryObjectPtr;
	UInventoryObject* InventoryObjectInstance;

	// 전리품 데이터 설정
	void SetLootData(const TArray<FAnimalLoot>& NewLoot);

	virtual void InteractionKeyPressed(AProject_GGFCharacter* Character) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
};
