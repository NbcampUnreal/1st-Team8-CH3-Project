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
   
    // Ÿ�Կ� ���� ���� �ð� ����
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

    // ��Ʈ�ѷ� ����
    // AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
    // if (AIController)
    // {
    //     AIController->StopMovement();
    //     AIController->UnPossess();  
    // }

    
    OwnerCharacter->SetActorHiddenInGame(true);
    OwnerCharacter->SetActorEnableCollision(false);

    // �÷��̾��� ��� �Է� ����
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
            FMath::RandRange(-1000.f, 1000.f),  // X ������
            FMath::RandRange(-1000.f, 1000.f),  // Y ������
            300.f                               // Z�� ����
        );
    }
    else  // AI �� �Ǵ� ������ ���
    {
        SpawnLocation = GetRandomSpawnPointNearOwnerCharacter();
    }

    FRotator SpawnRotation = FRotator::ZeroRotator;

    // ���� ĳ���� ��ġ ���� �� �ٽ� Ȱ��ȭ
    OwnerCharacter->SetActorLocation(SpawnLocation);
    OwnerCharacter->SetActorRotation(SpawnRotation);
    OwnerCharacter->SetActorHiddenInGame(false);
    OwnerCharacter->SetActorEnableCollision(true);

    // ü�� �ʱ�ȭ
    CurrentHealth = MaxHealth;
    bIsDead = false;

    // �÷��̾� & AI �и� ó��
    if (OwnerCharacter->ActorHasTag("Player"))
    {
        // �÷��̾��� ���: �Է� Ȱ��ȭ
        APlayerController* PlayerController = Cast<APlayerController>(Controller);
        if (PlayerController)
        {
            PlayerController->EnableInput(PlayerController);
        }
    }
    else
    {
       // AI �� �Ǵ� ����: AIController ����
       // AAIController* AIController = Cast<AAIController>(Controller);
       // if (AIController)
       // {
       //    AIController->RestartBehaviorTree();                          �÷��̾� ��Ʈ�ѷ��� �ִٸ�
       // }
    }

    GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
}


FVector UHealthComponent::GetRandomSpawnPointNearOwnerCharacter()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return FVector::ZeroVector;

    FVector TargetLocation = OwnerCharacter->GetActorLocation();

                                                        // �ݰ� 1000.0f ������ ������ X, Y ��ǥ ����
    float RandomX = FMath::RandRange(-1000.0f, 1000.0f);
    float RandomY = FMath::RandRange(-1000.0f, 1000.0f);
    FVector RandomLocation = TargetLocation + FVector(RandomX, RandomY, 0.0f);

                                                        // ���� ���̸� ����� Z �� ����
    FHitResult HitResult;
    FVector TraceStart = RandomLocation + FVector(0.0f, 0.0f, 500.0f); // ������ ����
    FVector TraceEnd = RandomLocation - FVector(0.0f, 0.0f, 1000.0f); // �Ʒ��� Raycast

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerCharacter);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
    {
                                                         // ���� ������ �����ϵ��� Z �� ����
        RandomLocation.Z = HitResult.Location.Z + 10.0f; // �ణ ����� ����
    }
    else
    {
                                                         // ������ �� ã���� �⺻ ���� ����
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

