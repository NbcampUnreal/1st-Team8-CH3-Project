#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "EnumKeyType.generated.h"

/*
*  AI의 감각들을 여기서 정리 선언 가능
*/
UENUM(BlueprintType)
enum class ESenseType : uint8
{
    Sight UMETA(DisplayName = "Sight"),
    Hearing UMETA(DisplayName = "Hearing")
};

/*
*  AI의 시야에 따른 행동 상태 열거 (나중에 플레이어 공격이랑 추격하는 상태 추가할 예정)
* 대기, 순찰, 경계, 플레이어 추적, 
*/
UENUM(BlueprintType)
enum class ELookState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Patrolling UMETA(DisplayName = "Patrolling"),
    Alert UMETA(DisplayName = "Alert"),
    PlayerTracking UMETA(DisplayName = "Player Tracking"),
    Investigating UMETA(DisplayName = "Investigating"),
    Searching UMETA(DisplayName = "Searching"),
    SweepingSurroundings UMETA(DisplayName = "Sweeping Surroundings"), 
    ReturnToHome UMETA(DisplayName = "Return To Home"),
    Alerted UMETA(DisplayName = "Alerted")
};

/*
*  소리 근원지 유형
*/
UENUM(BlueprintType)
enum class ENoiseSourceType : uint8
{
    SameSpecies UMETA(DisplayName = "Same Species"),
    Other UMETA(DisplayName = "Other")
};


UCLASS()
class UBlackboardKeyType_SenseType : public UBlackboardKeyType_Enum
{
    GENERATED_BODY()

public:
    UBlackboardKeyType_SenseType()
    {
        EnumType = StaticEnum<ESenseType>(); // Blackboard에서 ESenseType 인지하도록 설정
    }
};

UCLASS()
class UBlackboardKeyType_LookState : public UBlackboardKeyType_Enum
{
    GENERATED_BODY()

public:
    UBlackboardKeyType_LookState()
    {
        EnumType = StaticEnum<ELookState>(); // Blackboard에서 ELookState 인지하도록 설정
    }
};
