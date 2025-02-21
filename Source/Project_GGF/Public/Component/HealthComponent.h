#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"  
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Bullet,   // 총기 공격
    Melee,    // 근접 무기 공격
    Animal    // 동물 공격
};

class AAIController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GGF_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();


public:
    UPROPERTY(EditAnywhere, Category = "Health")
    int MaxHealth;

    UPROPERTY(VisibleAnywhere, Category = "Health")
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

    UPROPERTY(BlueprintReadOnly, Category = "Dead")
    bool bIsDead = false;

};
