
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractActor.generated.h"


UINTERFACE(MinimalAPI)
class UInteractActor : public UInterface
{
	GENERATED_BODY()
};


class PROJECT_GGF_API IInteractActor
{
	GENERATED_BODY()

public:
	
	virtual void Interact(AActor* Interactor) = 0;
};

