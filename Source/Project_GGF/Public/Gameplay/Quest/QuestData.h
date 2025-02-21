#pragma once

#include "CoreMinimal.h"
#include "QuestData.generated.h"

UENUM(BlueprintType)
enum class EQuestState : uint8
{
    InProgress, 
    Completed   
};

USTRUCT(BlueprintType)
struct FQuestData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TMap<FString, int32> TargetItems;  // 목표 아이템

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TMap<FString, int32> CurrentItems;  // 현재 수집한 아이템 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestState QuestState = EQuestState::InProgress;  
};