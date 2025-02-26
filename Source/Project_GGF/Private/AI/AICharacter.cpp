#include "AICharacter.h"
#include "AIControllerCustom.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h" // 발소리 감지를 확인하기위해 넣은 함수 나중에 같이 없앨것!!!!!!
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


 
    FootstepNoiseSettings.BaseFootstepLoudness = 3.0f;
    FootstepNoiseSettings.RunningLoudnessMultiplier = 1.5f;
    FootstepNoiseSettings.NoiseRadius = 1000.0f;
    FootstepNoiseSettings.MinSpeedForNoise = 100.0f;
    FootstepNoiseSettings.bIgnorableByAI = false;

    FootstepInterval = 0.5f;
    bGenerateMovementNoise = true;

}



void AAICharacter::BeginPlay()
{
    Super::BeginPlay();

    // 발소리가 true일 경우 타이머를 통해 발소리를 FootstepInterval주기로 MakeFootsetpNoise를 실행합니다
    if (bGenerateMovementNoise)
    {
        GetWorldTimerManager().SetTimer(
            FootstepNoiseTimerHandle,
            this,
            &AAICharacter::MakeFootstepNoise,
            FootstepInterval,
            true
        );
    }
    // 60초동안 발소리 감지 가능한 거리를 보여줍니다 디버그 용으로 나중에 삭제할겁니다
    ShowNoiseRange(60.0f);
}

/*
*  발걸음을 이벤트 시키는 함수입니다 Actor위치로부터의 loudness의 강도로 NoiseRadius반경에게 AIFootStep이라는 소음 이벤트를 전달합니다
*  // 현재 이동 속도가 MinSpeedForNoise보다 크거나 같을 경우 발소리가 들리게 합니다 ai의 움직임을 0~1로 clamp를 하여 0.7보다 높을 경우 
*   발소리의 감지를 RunningLoudnessMultiplier값 만큼 곱해서 더 크게 이벤트를 발동시킵니다
*/

void AAICharacter::MakeFootstepNoise()
{
    if (!bGenerateMovementNoise) return;

    // 현재 이동 속도
    float CurrentSpeed = GetVelocity().Size();

    if (CurrentSpeed >= FootstepNoiseSettings.MinSpeedForNoise)
    {
        float SpeedRatio = FMath::Clamp(CurrentSpeed / GetCharacterMovement()->MaxWalkSpeed, 0.0f, 1.0f);
        bool IsRunning = SpeedRatio > 0.7f;

        float Loudness = FootstepNoiseSettings.BaseFootstepLoudness;
        if (IsRunning)
        {
            Loudness *= FootstepNoiseSettings.RunningLoudnessMultiplier;
        }


        // Actor위치로부터의 loudness의 강도로 NoiseRadius반경에게 AIFootStep이라는 소음 이벤트를 전달합니다
        UAISense_Hearing::ReportNoiseEvent(
            GetWorld(),
            GetActorLocation(),
            Loudness,
            this,
            FootstepNoiseSettings.NoiseRadius,
            FName("AIFootStep")
        );

        UE_LOG(LogTemp, Verbose, TEXT("%s: 발소리 생성, 위치: %s, 강도: %.1f"),
            *GetName(), *GetActorLocation().ToString(), Loudness);


    }

}

void AAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    // 게임 종료시 발소리 내는 타이머를 종료시킵니다
    GetWorldTimerManager().ClearTimer(FootstepNoiseTimerHandle);
}


/*
*  발소리 범위 측정하기 위한 함수 나중에 삭제할것!!!!!!!!!!!!!!!!!
*/
void AAICharacter::ShowNoiseRange(float Duration)
{
    // 발소리 범위를 원형으로 시각화
    FVector Center = GetActorLocation();
    float Radius = FootstepNoiseSettings.NoiseRadius;

    // 바닥 위치 계산 (캐릭터 발 근처)
    FVector FloorCenter = Center - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 10.0f);

    // 원 그리기
    DrawDebugCircle(
        GetWorld(),
        FloorCenter,   // 원의 중심 (바닥 근처)
        Radius,        // 반지름
        32,            // 분할 수
        FColor::Cyan,  // 색상
        false,         // 지속 여부
        Duration,      // 표시 시간
        0,             // 우선순위
        2.0f           // 선 두께
    );

    // 중심점 표시
    DrawDebugPoint(
        GetWorld(),
        FloorCenter,
        10.0f,         // 점 크기
        FColor::Red,   // 색상
        false,         // 지속 여부
        Duration       // 표시 시간
    );

    // 정보 텍스트 표시
    DrawDebugString(
        GetWorld(),
        FloorCenter + FVector(0, 0, 30),
        FString::Printf(TEXT("소리 범위: %.1f"), Radius),
        nullptr,
        FColor::White,
        Duration
    );

    UE_LOG(LogTemp, Display, TEXT("%s의 발소리 범위(%.1f)를 표시했습니다"), *GetName(), Radius);
}