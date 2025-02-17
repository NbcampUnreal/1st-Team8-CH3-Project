#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animal.generated.h"

UCLASS()
class PROJECT_GGF_API AAnimal : public ACharacter
{
	GENERATED_BODY()

public:
	AAnimal();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
