#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

/*
*  ai 유형 정의 (ai를 서로 다르게 감지하기 위함)
*/

UENUM(BlueprintType)
enum class EAIType : uint8
{
	EAT_Creature UMETA(DisplayName = "Creature"),
	EAT_Poacher UMETA(DisplayName = "Poacher")
};

UCLASS()
class PROJECT_GGF_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

	// ai 유형 기본 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAIType AIType = EAIType::EAT_Creature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName AIName = TEXT("DefaultAIName");

};
