#include "Interact/TreasureChestInteractiveActor.h"
#include "Items/Inventory/InventoryObject.h"
#include "Project_GGF/Public/Controller/CharacterController.h"


ATreasureChestInteractiveActor::ATreasureChestInteractiveActor()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);



}

void ATreasureChestInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	InventoryObjectInstance = Cast<UInventoryObject>(InventoryObjectPtr.GetDefaultObject());

	if (InventoryObjectInstance)
	{
		InventoryObjectInstance->CreateChestInventory(GetWorld()->GetFirstPlayerController());
	}
}

void ATreasureChestInteractiveActor::InteractionKeyPressed(AProject_GGFCharacter* Character)
{
	ACharacterController* MyPlayerController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
	MyPlayerController->ShowInteractInventoryUI(InventoryObjectInstance->InventoryInstance);
}

void ATreasureChestInteractiveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ATreasureChestInteractiveActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	//Destroy();
}
