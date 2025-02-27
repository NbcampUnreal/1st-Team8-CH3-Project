#pragma once

#include "CoreMinimal.h"
#include "AI/Creatures/Animal.h"
#include "DeerDoe.generated.h"


UCLASS()
class PROJECT_GGF_API ADeerDoe : public AAnimal
{
	GENERATED_BODY()

public:
    FVector CalculateAveragePos();

    UPROPERTY(EditAnywhere, Category = "Deer")
    float HerdRadius = 1000.0f;

protected:
    virtual void Tick(float DeltaTime) override;

    void FindNearbyDeer();

    TArray<AActor*> NearbyDeer;
};
