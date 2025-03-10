#pragma once

#include "CoreMinimal.h"
#include "Character/GGFCharacterBase.h"
#include "GGFAICharacterBase.generated.h"

UCLASS()
class PROJECT_GGF_API AGGFAICharacterBase : public AGGFCharacterBase
{
	GENERATED_BODY()

public:
	AGGFAICharacterBase();
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void OnDie() override;
	virtual void OnHit(AActor* Attacker) override;
	void UpdateAttackState(bool bIsHit, const FVector& AttackerLocation);

private:
	void ResetAttackState();

	class UBlackboardComponent* BlackboardComponent;
	FTimerHandle AttackResetTimerHandle;
};
