#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NavigationSystem.h"  
#include "AIController.h"      
#include "Navigation/PathFollowingComponent.h"  
#include "BTTask_Search.generated.h"
UCLASS()
class UBTTask_Search : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_Search();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    // 수색 범위 설정 
    UPROPERTY(EditAnywhere, Category = "AI Search")
    float SearchRadius = 500.0f;
    // 수색 지속 시간 설정 
    UPROPERTY(EditAnywhere, Category = "AI Search")
    float SearchDuration = 5.0f;
    UPROPERTY(EditAnywhere, Category = "AI Search")
    int32 MaxSearchAttempts = 5;
    // 경과 시간 추적
    float ElapsedTime = 0.0f;
    void SearchTimerCallback();

    int32 mSearchIndex;
    TArray<FVector> mSearchPoints;
    AAIController* mAIController;
    UBlackboardComponent* mBlackboardComp;
    UBehaviorTreeComponent* mOwnerComp;
    FTimerHandle mSearchTimerHandle;  // 추가된 멤버 변수
};