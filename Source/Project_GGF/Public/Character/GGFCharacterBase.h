#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GGFCharacterBase.generated.h"

UCLASS()
class PROJECT_GGF_API AGGFCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGGFCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
