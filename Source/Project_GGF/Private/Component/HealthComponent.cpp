#include "Project_GGF/Public/Component/HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    MaxHealth = 100;
    CurrentHealth = MaxHealth;
}



void UHealthComponent::TakeDamage(AActor* Attacker, EAttackType AttackType, float StiffTime, int HealthAmount)
{
    if (!Attacker || HealthAmount <= 0 || IsDead()) return;

 
    CurrentHealth = FMath::Clamp(CurrentHealth - HealthAmount, 0, MaxHealth);
   
    // 타입에 따른 경직 시간 변경
    if (AttackType == EAttackType::Melee)
    {
        StiffTime *= 1.5f; 
    }
    else if (AttackType == EAttackType::Animal)
    {
        StiffTime *= 2.0f; 
    }
    
    if (StiffTime > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            StiffTimerHandle,
            this,
            &UHealthComponent::EndStiffTime,
            StiffTime,
            false
        );
    }

   
    if (IsDead())
    {
		OnDeath();
    }
}


void UHealthComponent::Heal(int HealAmount)
{
    if (HealAmount > 0 && !IsDead())
    {
        CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0, MaxHealth);
       
    }
}

void UHealthComponent::OnDeath()
{
    if (bIsDead) return;
    bIsDead = true;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // 컨트롤러 해제
    // AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
    // if (AIController)
    // {
    //     AIController->StopMovement();
    //     AIController->UnPossess();  
    // }

    
    OwnerCharacter->SetActorHiddenInGame(true);
    OwnerCharacter->SetActorEnableCollision(false);

    // 플레이어인 경우 입력 차단
    APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
    if (PlayerController)
    {
        PlayerController->DisableInput(PlayerController);
    }

    // Respawn
    if (bCanRespawn)
    {
        GetWorld()->GetTimerManager().SetTimer(
            RespawnTimerHandle,
            this,
            &UHealthComponent::Respawn,
            5.0f,
            false
        );
    }
    // Destroy
    else
    {
      
        GetWorld()->GetTimerManager().SetTimer(
            DeathCleanupTimerHandle,
            this,
            &UHealthComponent::DestroyOwner,
            10.0f,
            false
        );
    }
}
////////////////////////////////////////////////////////////////
void UHealthComponent::Respawn()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    AController* Controller = OwnerCharacter->GetController();

   
    FVector SpawnLocation;
    if (OwnerCharacter->ActorHasTag("Player"))
    {
        SpawnLocation = FVector(
            FMath::RandRange(-1000.f, 1000.f),  // X 랜덤값
            FMath::RandRange(-1000.f, 1000.f),  // Y 랜덤값
            300.f                               // Z는 고정
        );
    }
    else  // AI 적 또는 동물인 경우
    {
        SpawnLocation = GetRandomSpawnPointNearOwnerCharacter();
    }

    FRotator SpawnRotation = FRotator::ZeroRotator;

    // 기존 캐릭터 위치 변경 후 다시 활성화
    OwnerCharacter->SetActorLocation(SpawnLocation);
    OwnerCharacter->SetActorRotation(SpawnRotation);
    OwnerCharacter->SetActorHiddenInGame(false);
    OwnerCharacter->SetActorEnableCollision(true);

    // 체력 초기화
    CurrentHealth = MaxHealth;
    bIsDead = false;

    // 플레이어 & AI 분리 처리
    if (OwnerCharacter->ActorHasTag("Player"))
    {
        // 플레이어의 경우: 입력 활성화
        APlayerController* PlayerController = Cast<APlayerController>(Controller);
        if (PlayerController)
        {
            PlayerController->EnableInput(PlayerController);
        }
    }
    else
    {
       // AI 적 또는 동물: AIController 리셋
       // AAIController* AIController = Cast<AAIController>(Controller);
       // if (AIController)
       // {
       //    AIController->RestartBehaviorTree();                          플레이어 컨트롤러가 있다면
       // }
    }

    GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
}


FVector UHealthComponent::GetRandomSpawnPointNearOwnerCharacter()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return FVector::ZeroVector;

    FVector TargetLocation = OwnerCharacter->GetActorLocation();

                                                        // 반경 1000.0f 내에서 랜덤한 X, Y 좌표 선택
    float RandomX = FMath::RandRange(-1000.0f, 1000.0f);
    float RandomY = FMath::RandRange(-1000.0f, 1000.0f);
    FVector RandomLocation = TargetLocation + FVector(RandomX, RandomY, 0.0f);

                                                        // 지형 높이를 고려한 Z 값 조정
    FHitResult HitResult;
    FVector TraceStart = RandomLocation + FVector(0.0f, 0.0f, 500.0f); // 위에서 시작
    FVector TraceEnd = RandomLocation - FVector(0.0f, 0.0f, 1000.0f); // 아래로 Raycast

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerCharacter);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
    {
                                                         // 지형 위에서 스폰하도록 Z 값 조정
        RandomLocation.Z = HitResult.Location.Z + 10.0f; // 약간 띄워서 스폰
    }
    else
    {
                                                         // 지형을 못 찾으면 기본 높이 유지
        RandomLocation.Z = TargetLocation.Z + 300.0f;
    }

    return RandomLocation;
}

void UHealthComponent::DestroyOwner()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        OwnerCharacter->Destroy();
    }
}


int UHealthComponent::GetCurrentHealth()
{ return CurrentHealth; }

bool UHealthComponent::IsDead() 
{ return CurrentHealth <= 0; } 

void UHealthComponent::EndStiffTime()
{
    StiffTimerHandle.Invalidate();
}

