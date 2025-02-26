#include "AICharacter.h"
#include "AIControllerCustom.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacter::AAICharacter()
	: AIType(EAIType::EAT_Creature)  // 멤버 초기화 리스트
	, AIName(TEXT("DefaultAIName"))
{
	AIControllerClass = AAIControllerCustom::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = 350.0f; // ai 기본 이동 속도 설정
        GetCharacterMovement()->BrakingDecelerationWalking = 500.0f; //  감속 속도 증가
        GetCharacterMovement()->BrakingFrictionFactor = 1.5f; //  감속 효과 조절
        GetCharacterMovement()->GroundFriction = 4.0f; //  지면 마찰력 증가

        UE_LOG(LogTemp, Warning, TEXT("AICharacter Movement Component 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ERROR: GetCharacterMovement()가 NULL입니다!"));
    }

}
