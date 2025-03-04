#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitDeadComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GGF_API UHitDeadComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeadMontage;


public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayHitMontage();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayDeadMontage();
		
};
