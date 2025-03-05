#include "AI/NPC/GGFAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AGGFAICharacter::AGGFAICharacter()
{
}

void AGGFAICharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponManager = Cast<UWeaponManager>(WeaponManagerPtr.GetDefaultObject());
	if (WeaponManager)
	{
		WeaponManager->CreateWeapons(this);
	}

	// InventoryObjectInstance = Cast<UInventoryObject>(InventoryObjectPtr.GetDefaultObject());
	//
	// if (InventoryObjectInstance)
	// {
	// 	InventoryObjectInstance->CreateEnemyInventory(GetController());
	// }

}

void AGGFAICharacter::Aiming()
{
	if (WeaponManager)
	{
		WeaponManager->AttachToHand();
	}
}

void AGGFAICharacter::Shoot()
{
	if (WeaponManager)
	{
		WeaponManager->Attack();
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (WeaponManager)
			{
				WeaponManager->AttachToBack();
			}
		}, 0.2f, false);
	}
}

void AGGFAICharacter::SetLootLocation(FVector NewLocation)
{
	LootLocation = NewLocation;

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool(TEXT("IsLootAvailable"), true);
		}
	}
}
