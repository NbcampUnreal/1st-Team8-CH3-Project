#include "AI/BT/BTDecorator_IsLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Data/HealthComponent.h"
#include "AI/GGFAICharacterBase.h"
#include "AI/GGFAIController.h"

bool UBTDecorator_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	AGGFAICharacterBase* AICharacter = Cast<AGGFAICharacterBase>(AIController->GetPawn());
	if (!AICharacter) return false;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return false;

	UHealthComponent* HealthComponent = AICharacter->FindComponentByClass<UHealthComponent>();
	if (!HealthComponent) return false;

	int CurrentHealth = HealthComponent->GetCurrentHealth();
	return (CurrentHealth >= MinHealth && CurrentHealth <= MaxHealth);
}
