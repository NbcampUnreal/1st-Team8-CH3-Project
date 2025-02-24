#include "Items/UtiliyItem/HealingItem.h"


AHealingItem::AHealingItem()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AHealingItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemDataManagerClass != nullptr)
	{
		ItemDataManager = ItemDataManagerClass.GetDefaultObject();
	}

	if (ItemDataManager != nullptr)
	{
		FHealingItemDataTable* Data = ItemDataManager->GetHealingItemDataTable(ItemName);
		HealingType = Data->HealingType;
		Amount = Data->Amount;
		Duration = Data->Duration;
	}
}

void AHealingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealingItem::UseItem()
{
	// 상호작용

	DestroyItem();
}
