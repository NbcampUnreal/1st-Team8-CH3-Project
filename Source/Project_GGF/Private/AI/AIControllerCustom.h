#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnumKeyType.h" 
#include "AIControllerCustom.generated.h"
class AAICharacter;
UCLASS()
class PROJECT_GGF_API AAIControllerCustom : public AAIController
{
    GENERATED_BODY()
public:
    AAIControllerCustom();
    UPROPERTY(VisibleAnywhere, Category = "AI Perception")
    UAIPerceptionComponent* AIPerception;
    UPROPERTY(VisibleAnywhere, Category = "AI Perception")
    UAISenseConfig_Sight* SightConfig;

    void SetPatrolPos(const FVector& NewPatrolPos);
    FVector GetPatrolPos() const;
    void SetHomePos(const FVector& NewHomePos);
    FVector GetHomePos() const;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;
    virtual void Tick(float DeltaSeconds) override;

    bool CanTransitionToState(ELookState CurrentState, ELookState NewState);
    void HandleStateTransition(ELookState FromState, ELookState ToState);
    void ValidateAIState();
    void ForceInvestigate();
    void UpdateLookState(ELookState NewState);

    FRotator SmoothRotateTowards(const FRotator& CurrentRotation, const FRotator& TargetRotation, float DeltaSeconds);
    UFUNCTION()
    bool IsInSight(const FVector& TargetLocation) const;
    UFUNCTION()
    bool IsInSightIgnoreAngle(const FVector& TargetLocation, bool bCheckAngle) const;
    UFUNCTION()
    void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
    UFUNCTION()
    void UpdateSenseData(ESenseType SenseType, bool bDetected, const FVector& Location);
    UFUNCTION(BlueprintCallable)
    FSenseData GetSenseData(ESenseType SenseType) const;

    static const FName PatrolPosKey;
    static const FName TargetKey;
    static const FName HomePosKey;
protected:
    UPROPERTY()
    TMap<ESenseType, FSenseData> SenseDataMap; // 각 감각 유형별 데이터 저장

    // AI 감지 관련 설정값들
    UPROPERTY(EditAnywhere, Category = "AI Detection")
    float DetectionRadius = 1000.0f; // 감지 반경
    UPROPERTY(EditAnywhere, Category = "AI Detection")
    float MaxSightAngle = 60.0f; // ai 최대 시야각 
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBlackboardData* BBAsset;
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBehaviorTree* BTAsset;
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void DebugLogSenseData(ESenseType SenseType) const;
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void DrawDebugSenseInfo();
private:
    void InitializeSenseData();
    UPROPERTY()
    AAICharacter* ControlledAICharacter;
    UPROPERTY()
    ELookState CurrentLookState = ELookState::Idle;

    FTimerHandle PlayerInSightTimerHandle;
    FTimerHandle PlayerTrackingTimerHandle;

    void OnPlayerInSightTimerFinished();
    void OnPlayerTrackingTimerFinished();
    void StartPlayerInSightTimer();
    void StartPlayerTrackingTimer();

};