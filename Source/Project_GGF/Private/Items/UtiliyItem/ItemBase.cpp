#include "Items/UtiliyItem/ItemBase.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("_SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);

}