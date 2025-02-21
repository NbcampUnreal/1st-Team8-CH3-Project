#include "Project_GGF/Public/Component/RespawnComponent.h"
#include "Project_GGF/Public/Component/HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


URespawnComponent::URespawnComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

    RespawnTime = 5.0f;
}


void URespawnComponent::BeginPlay()
{
	Super::BeginPlay();


}

void URespawnComponent::Respawn()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

   
    RespawnLocation = FVector(
        FMath::RandRange(-1000.f, 1000.f),
        FMath::RandRange(-1000.f, 1000.f),
        300.f
    );
    RespawnRotation = FRotator::ZeroRotator;

   
    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        ACharacter* NewCharacter = World->SpawnActor<ACharacter>(OwnerCharacter->GetClass(), RespawnLocation, RespawnRotation, SpawnParams);

        if (NewCharacter)
        {
            
            NewCharacter->SetActorLocation(RespawnLocation);
            NewCharacter->SetActorRotation(RespawnRotation);

            
            UHealthComponent* NewHealthComp = NewCharacter->FindComponentByClass<UHealthComponent>();
            if (NewHealthComp)
            {
                NewHealthComp->CurrentHealth = NewHealthComp->MaxHealth;
                NewHealthComp->bIsDead = false;
            }

           
            APlayerController* PlayerController = Cast<APlayerController>(NewCharacter->GetController());
            if (PlayerController)
            {
                PlayerController->EnableInput(PlayerController);
            }
        }
    }

    DestroyOwner();

    GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
}


void URespawnComponent::DestroyOwner()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        OwnerCharacter->Destroy();
    }
}

