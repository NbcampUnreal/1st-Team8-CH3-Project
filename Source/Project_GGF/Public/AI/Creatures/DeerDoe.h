#pragma once

#include "CoreMinimal.h"
#include "AI/Creatures/Animal.h"
#include "DeerDoe.generated.h"


UCLASS()
class PROJECT_GGF_API ADeerDoe : public AAnimal
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsLeader;
};
