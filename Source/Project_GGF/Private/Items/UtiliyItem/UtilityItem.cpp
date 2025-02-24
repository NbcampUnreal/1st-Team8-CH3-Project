#include "Items/UtiliyItem/UtilityItem.h"

AUtilityItem::AUtilityItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);

	

}

void AUtilityItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUtilityItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

