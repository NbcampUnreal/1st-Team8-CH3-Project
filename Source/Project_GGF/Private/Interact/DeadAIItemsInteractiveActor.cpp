#include "Interact/DeadAIItemsInteractiveActor.h"
#include "AI/NPC/GGFAICharacter.h"
#include "Character/Project_GGFCharacter.h"
#include "Items/Inventory/InventoryObject.h"
#include "Project_GGF/Public/Controller/CharacterController.h"

void ADeadAIItemsInteractiveActor::SetInventoryData(UInventoryObject* InventoryObj)
{
	InventoryObjectInstance = InventoryObj;
}

void ADeadAIItemsInteractiveActor::InteractionKeyPressed(AActor* Actor)
{
	if (!Actor) return;

	ACharacterController* MyPlayerController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
	MyPlayerController->ShowInteractInventoryUI(Cast<UUserWidget>(InventoryObjectInstance));
	AProject_GGFCharacter* Character = Cast<AProject_GGFCharacter>(Actor);
	if (Character)
	{
		Character->EndInteract();
	}
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
