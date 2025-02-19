#include "AI/Creatures/BTService_UpdateTime.h"
#include "Gameplay/GGFGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateTime::UBTService_UpdateTime()
{
	Interval = 1.0f;
}

void UBTService_UpdateTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return;

    AGGFGameMode* GameMode = Cast<AGGFGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    float TimeRatio = GameMode->GetGameTimeRatio();

    bool bIsDay = (TimeRatio <= 0.4f);
    bool bIsEvening = (TimeRatio > 0.4f && TimeRatio < 0.7f);
    bool bIsNight = (TimeRatio >= 0.7f);

    Blackboard->SetValueAsBool(TEXT("bIsDay"), bIsDay);
    Blackboard->SetValueAsBool(TEXT("bIsEvening"), bIsEvening);
    Blackboard->SetValueAsBool(TEXT("bIsNight"), bIsNight);
}
