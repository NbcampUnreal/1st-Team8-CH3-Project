#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShotgunBullet.generated.h"

UCLASS()
class PROJECT_GGF_API AShotgunBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AShotgunBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
