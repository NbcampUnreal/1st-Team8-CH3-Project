#include "Interact/DeadAIItemsInteractiveActor.h"
#include "AI/NPC/GGFAICharacter.h"
#include "Character/Project_GGFCharacter.h"
#include "Items/Inventory/InventoryObject.h"
#include "Project_GGF/Public/Controller/CharacterController.h"

void ADeadAIItemsInteractiveActor::SetInventoryData(UInventoryObject* InventoryObj)
{
	InventoryObjectInstance = InventoryObj;
}

void ADeadAIItemsInteractiveActor::InteractionKeyPressed(AProject_GGFCharacter* Character)
{
	if (!Character) return;

	ACharacterController* MyPlayerController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
	MyPlayerController->ShowInteractInventoryUI(Cast<UUserWidget>(InventoryObjectInstance));
}

void ADeadAIItemsInteractiveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void ADeadAIItemsInteractiveActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);


	Destroy();
}
