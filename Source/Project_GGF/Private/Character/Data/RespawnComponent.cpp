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

        // 기존 캐릭터의 컨트롤러를 저장
        AController* PlayerController = OwnerCharacter->GetController();
        
        // 새 캐릭터 생성
        ACharacter* NewCharacter = World->SpawnActor<ACharacter>(
            OwnerCharacter->GetClass(), RespawnLocation, RespawnRotation, SpawnParams
        );

        if (NewCharacter)
        {
            // 기존 캐릭터 제거
            OwnerCharacter->Destroy();

            // 기존 플레이어 컨트롤러가 있다면 새 캐릭터로 Possess 시도
            if (PlayerController)
            {
                PlayerController->Possess(NewCharacter);
            }

            // 위치 및 상태 초기화
            NewCharacter->SetActorLocation(RespawnLocation);
            NewCharacter->SetActorRotation(RespawnRotation);

            // 체력 초기화
            UHealthComponent* NewHealthComp = NewCharacter->FindComponentByClass<UHealthComponent>();
            if (NewHealthComp)
            {
                NewHealthComp->CurrentHealth = NewHealthComp->MaxHealth;
                NewHealthComp->bIsDead = false;
            }

            // 입력 활성화
            APlayerController* NewPlayerController = Cast<APlayerController>(PlayerController);
            if (NewPlayerController)
            {
                NewPlayerController->EnableInput(NewPlayerController);
            }
        }
    }

    // 타이머 제거
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

