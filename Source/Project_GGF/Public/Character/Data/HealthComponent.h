#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"  
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

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, Category = "Health")
    UDataTable* HealthDataTable;  // 데이터 테이블

    UPROPERTY(EditAnywhere, Category = "Health")
    FName HealthDataRowName;  // 데이터 테이블에서 가져올 행 이름

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

    UPROPERTY(BlueprintReadOnly, Category = "Dead")
    bool bIsDead = false;

    void LoadHealthData();

};
