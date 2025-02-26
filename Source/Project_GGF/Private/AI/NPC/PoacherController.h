#pragma once

#include "CoreMinimal.h"
#include "AI/AIControllerCustom.h"
#include "PoacherController.generated.h"

UCLASS()
class APoacherController : public AAIControllerCustom
{
	GENERATED_BODY()
	
public:
	APoacherController();

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxJumpHeight = 100.0f;
	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectiveDistance = 200.0f;

protected:

	virtual void Tick(float DeltaTime) override;

private:
	void DetectObstacle();

};
