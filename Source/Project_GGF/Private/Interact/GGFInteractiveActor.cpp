#include "Interact/GGFInteractiveActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/Project_GGFCharacter.h"

AGGFInteractiveActor::AGGFInteractiveActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Interactive Actor"));
	RootComponent = Root;

	WidgetTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Widget Trigger"));
	WidgetTriggerBox->SetupAttachment(RootComponent);
	WidgetTriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	NotifyInteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	NotifyInteractionWidget->SetupAttachment(RootComponent);
}

void AGGFInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	NotifyInteractionWidget->SetVisibility(false);
	
	WidgetTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGGFInteractiveActor::OnOverlapBegin);
	WidgetTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGGFInteractiveActor::OnOverlapEnd);
}

void AGGFInteractiveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AProject_GGFCharacter* Player = Cast<AProject_GGFCharacter>(OtherActor);
	{
		if (Player)
		{
			
			NotifyInteractionWidget->SetVisibility(true);
			
			 Player->InteractableActor = this;  
		}
	}
}

void AGGFInteractiveActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AProject_GGFCharacter* Player = Cast<AProject_GGFCharacter>(OtherActor);
	{
		if (Player)
		{
			
			NotifyInteractionWidget->SetVisibility(false);
			
			InteractableActor = nullptr;

			Player->InteractableActor = nullptr;

		}
	}
}

void AGGFInteractiveActor::InteractionKeyPressed(AProject_GGFCharacter* Character)
{
}

void AGGFInteractiveActor::ShowInteractionWidget(bool bShow)
{
	if (NotifyInteractionWidget)
	{
		NotifyInteractionWidget->SetVisibility(bShow);
	}
}



