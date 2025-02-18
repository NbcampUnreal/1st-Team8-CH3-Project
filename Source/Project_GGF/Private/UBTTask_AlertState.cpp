#include "UBTTask_AlertState.h"
#include "AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UUBTTask_AlertState::UUBTTask_AlertState()
{
	NodeName = TEXT("AlertState");
	bNotifyTick = true; // Tick 불러오기
}

EBTNodeResult::Type UUBTTask_AlertState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerCustom* AIController = Cast<AAIControllerCustom>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		// AI가 경계 상태일 경우 이동을 멈춤
		ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
		if (Character)
		{
			UCharacterMovementComponent* CharMovementComponent = Character->GetCharacterMovement();
			if (CharMovementComponent)
			{
				CharMovementComponent->StopMovementImmediately();
			}
		}
	}

	// 시간 0으로 초기화
	float* ElapsedTime = reinterpret_cast<float*>(NodeMemory);
	*ElapsedTime = 0.0f;
	bPlayerInSight = true;
	
	return EBTNodeResult::InProgress;
}

void UUBTTask_AlertState::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIControllerCustom* AIController = Cast<AAIControllerCustom>(OwnerComp.GetOwner());
	if (AIController)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIController, 0);
		if (PlayerPawn)
		{
			APawn* AIPawn = AIController->GetPawn();
			if (AIPawn)
			{
				// 플레이어가 시야에 있는지 체크
				FVector ToPlayer = PlayerPawn->GetActorLocation() - AIPawn->GetActorLocation();
				ToPlayer.Normalize();
				FVector ForwardVector = AIPawn->GetActorForwardVector();

				float DotProduct = FVector::DotProduct(ForwardVector, ToPlayer);
				if (DotProduct > 0.5f)
				{
					bPlayerInSight = true;
					// 플레이어를 향해 회전
					FRotator TargetRotation = ToPlayer.Rotation();
					FRotator NewControlRotation = FMath::RInterpTo(AIController->GetControlRotation(), TargetRotation, DeltaSeconds, 5.0f);
					AIController->SetControlRotation(NewControlRotation);
					
					float* ElapsedTime = reinterpret_cast<float*>(NodeMemory);
					*ElapsedTime = 0.0f;

				}
				else
				{
					bPlayerInSight = false;
				}
			}
		}

	}
	// 플레이어가 안보일 때 경과 시간 증가
	if (!bPlayerInSight)
	{
		float* ElapsedTime = reinterpret_cast<float*>(NodeMemory);
		*ElapsedTime += DeltaSeconds;

		if (*ElapsedTime >= LostSightDelay)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

	}

}

uint16 UUBTTask_AlertState::GetInstanceMemorySize() const
{
	//경과 시간 저장
	return sizeof(float);
}