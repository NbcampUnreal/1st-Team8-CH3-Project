#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Bullet,   // �ѱ� ����
    Melee,    // ���� ���� ����
    Animal    // ���� ����
};

class AAIController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GGF_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();


private:
    UPROPERTY(EditAnywhere, Category = "Health")
    int MaxHealth;

    UPROPERTY(VisibleAnywhere, Category = "Health")
    int CurrentHealth;

    FTimerHandle StiffTimerHandle;
    FTimerHandle RespawnTimerHandle;
    FTimerHandle DeathCleanupTimerHandle;

public:
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void TakeDamage(AActor* Attacker, EAttackType AttackType, float StiffTime, int HealthAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(int HealAmount);

    void EndStiffTime();

    UFUNCTION(BlueprintPure, Category = "Health")
    int GetCurrentHealth();

    UFUNCTION(BlueprintPure, Category = "Dead")
    bool IsDead();
    void OnDeath();
    void Respawn();
    void DestroyOwner();
    FVector GetRandomSpawnPointNearOwnerCharacter();

    UPROPERTY(BlueprintReadOnly, Category = "Dead")
    bool bIsDead = false;
    bool bCanRespawn = true;

};
