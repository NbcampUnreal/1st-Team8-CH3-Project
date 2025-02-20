
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GGF_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	URespawnComponent();

	virtual void BeginPlay() override;
public:

};
