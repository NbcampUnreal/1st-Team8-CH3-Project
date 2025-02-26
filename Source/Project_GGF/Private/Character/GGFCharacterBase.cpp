#include "Character/GGFCharacterBase.h"
#include "AI/Creatures/Animal.h"

AGGFCharacterBase::AGGFCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGGFCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGGFCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGGFCharacterBase::AddLootToInventory(const TArray<FAnimalLoot>& LootItems)
{
    for (const FAnimalLoot& Item : LootItems)
    {
        bool bItemExists = false;

        for (FAnimalLoot& ExistingItem : Inventory)
        {
            if (ExistingItem.ItemID == Item.ItemID)
            {
                ExistingItem.Quantity += Item.Quantity; 
                bItemExists = true;
                break;
            }
        }

        if (!bItemExists)
        {
            Inventory.Add(Item);
        }
    }
}

TArray<FAnimalLoot> AGGFCharacterBase::GetInventoryLoot() const
{
    return Inventory;
}

