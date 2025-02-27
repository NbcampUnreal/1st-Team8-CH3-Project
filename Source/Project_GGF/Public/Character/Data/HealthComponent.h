#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"  
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Bullet,  
    Melee,    
    Animal    
};

class AAIController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GGF_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

public:
    int MaxHealth;
    int CurrentHealth;


    FTimerHandle StiffTimerHandle;

public:
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void TakeDamage(AActor* Attacker, EAttackType AttackType, float StiffTime, int HealthAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(int HealAmount);

    UFUNCTION(BlueprintPure, Category = "Health")
    int GetCurrentHealth(){return CurrentHealth;}

    UFUNCTION(BlueprintPure, Category = "Dead")
    bool IsDead() { return CurrentHealth <= 0; };
    void OnDeath();
    void EndStiffTime() { StiffTimerHandle.Invalidate(); }
    void HandleLootDrop(const FVector& DeathLocation);
    void HandleRespawn(ACharacter* OwnerCharacter);

    UPROPERTY(BlueprintReadOnly, Category = "Dead")
    bool bIsDead = false;

    void LoadHealthData();

    void SetLastAttacker(AActor* Attacker) { LastAttacker = Attacker; }
    AActor* GetLastAttacker() const { return LastAttacker; }

private:
    AActor* LastAttacker;
};
