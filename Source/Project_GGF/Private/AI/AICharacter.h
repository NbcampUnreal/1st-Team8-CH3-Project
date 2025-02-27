#pragma once
#include "CoreMinimal.h"
#include "AI/GGFAICharacterBase.h"
#include "Items/Manager/WeaponManager.h"
#include "AICharacter.generated.h"

/*
*  AI 유형 정의 (AI를 서로 다르게 감지하기 위함)
*/
UENUM(BlueprintType)
enum class EAIType : uint8
{
    EAT_Creature UMETA(DisplayName = "Creature"),
    EAT_Poacher UMETA(DisplayName = "Poacher")
};

/*
*  AI 발소리 설정
*/
USTRUCT(BlueprintType)
struct FFootstepNoiseSettings
{
    GENERATED_BODY()

    // 기본 발소리 강도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float BaseFootstepLoudness = 3.0f;

    // 달릴 때 발소리 강도 증가 계수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound", meta = (ClampMin = "1.0", ClampMax = "3.0"))
    float RunningLoudnessMultiplier = 1.5f;

    // 소리 발생 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    float NoiseRadius = 1000.0f;

    // 이 값 이상일 때 소리 발생
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    float MinSpeedForNoise = 100.0f;

    // 다른 AI가 이 소리를 무시할지 여부 (같은 종족이 아니더라도)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    bool bIgnorableByAI = false;
};

UCLASS()
class PROJECT_GGF_API AAICharacter : public AGGFAICharacterBase
{
    GENERATED_BODY()

public:
    AAICharacter();
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // AI 유형 기본 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EAIType AIType = EAIType::EAT_Creature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName AIName = TEXT("DefaultAIName");

    // 발소리 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    FFootstepNoiseSettings FootstepNoiseSettings;

    // 발소리 타이머 핸들
    FTimerHandle FootstepNoiseTimerHandle;

    // 발소리 간격
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    float FootstepInterval = 0.5f;

    // 발소리 만들기 함수
    UFUNCTION(BlueprintCallable, Category = "AI|Sound")
    void MakeFootstepNoise();

    // 이동 기반 발소리 생성 활성 / 비활성화
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sound")
    bool bGenerateMovementNoise = true;

    // 소리 거리를 보여줍니다
    UFUNCTION(BlueprintCallable, Category = "AI|Debug")
    void ShowNoiseRange(float Duration = 5.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UWeaponManager> WeaponManagerPtr;

	UWeaponManager* WeaponManager;

	TArray<FName> HandSockets;
	TArray<FName> BackSockets;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetHandSockets() const { return HandSockets; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TArray<FName> GetBackSockets() const { return BackSockets; }

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireMontage;
		
private:
	FVector LootLocation;  // 전리품 위치

public:
	void SetLootLocation(FVector NewLocation) { LootLocation = NewLocation; }
	FVector GetLootLocation() const { return LootLocation; };
	void ClearLootLocation() { LootLocation = FVector::ZeroVector; }

	void AddItemToInventory();
};
