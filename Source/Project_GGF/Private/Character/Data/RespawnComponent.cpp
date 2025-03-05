#include "Project_GGF/Public/Character/Data/RespawnComponent.h"
#include "Project_GGF/Public/Character/Data/HealthComponent.h"
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

void URespawnComponent::Spawn()
{
}

void URespawnComponent::Respawn()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    RespawnLocation = FVector(-2631.0f, -1462.0f, -7705.0f);
    RespawnRotation = FRotator::ZeroRotator;

    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        AController* PlayerController = OwnerCharacter->GetController();
        
        ACharacter* NewCharacter = World->SpawnActor<ACharacter>(
            OwnerCharacter->GetClass(), RespawnLocation, RespawnRotation, SpawnParams
        );

        if (NewCharacter)
        {
            
            OwnerCharacter->Destroy();
            if (PlayerController)
            {
                PlayerController->Possess(NewCharacter);
            }
            NewCharacter->SetActorLocation(RespawnLocation);
            NewCharacter->SetActorRotation(RespawnRotation);
            
            UHealthComponent* NewHealthComp = NewCharacter->FindComponentByClass<UHealthComponent>();
            if (NewHealthComp)
            {
                NewHealthComp->CurrentHealth = NewHealthComp->MaxHealth;
                NewHealthComp->bIsDead = false;
            }
            APlayerController* NewPlayerController = Cast<APlayerController>(PlayerController);
            if (NewPlayerController)
            {
                NewPlayerController->EnableInput(NewPlayerController);
            }
        }
    }
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

