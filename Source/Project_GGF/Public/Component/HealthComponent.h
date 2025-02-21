#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
    Melee UMETA(DisplayName = "Melee"),
    Ranged UMETA(DisplayName = "Ranged"),
    Explosion UMETA(DisplayName = "Explosion")
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GGF_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Health")
    UDataTable* HealthDataTable;  // 데이터 테이블

    UPROPERTY(EditAnywhere, Category = "Health")
    FName HealthDataRowName;  // 데이터 테이블에서 가져올 행 이름

    int MaxHealth;
    int CurrentHealth;

    FTimerHandle StiffTimerHandle;

public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(ACharacter* Attacker, EDamageType DamageType, float StiffTime, int HealthAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(int HealAmount);

    void EndStiffTime();

    UFUNCTION(BlueprintPure, Category = "Health")
    int GetCurrentHealth();

    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsDead();

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    bool bIsDead = false;

    void LoadHealthData();

};
