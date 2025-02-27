#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AISenseManager.generated.h"

class UBlackboardComponent;
class UAIStateManager;
class AAIControllerCustom;
class UAISightHandler;

USTRUCT(BlueprintType)
struct FSenseData
{
    GENERATED_BODY()

    // ai 적 감지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsDetected = false;

    // 마지막으로 감지한 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LastDetectionTime = 0.0f;

    // 마지막 감지때의 대상의 위치
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector LastKnownLocation = FVector::ZeroVector;
};

UCLASS()
class PROJECT_GGF_API UAISenseManager : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize(UBlackboardComponent* InBlackboard, AAIControllerCustom* InController);

    // 감각 데이터 관리
    void UpdateSenseData(ESenseType SenseType, bool bDetected, const FVector& Location);
    FSenseData GetSenseData(ESenseType SenseType) const;

    // 감각 디버깅
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void DebugLogSenseData(ESenseType SenseType) const;

    void DrawDebugSenseInfo();

private:
    UPROPERTY()
    UBlackboardComponent* BlackboardComp;

    UPROPERTY()
    UAIStateManager* StateManager;

    UPROPERTY()
    AAIControllerCustom* AIController;

public:
    UPROPERTY()
    TMap<ESenseType, FSenseData> SenseDataMap;
};
