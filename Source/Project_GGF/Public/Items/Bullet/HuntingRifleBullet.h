
#pragma once

#include "CoreMinimal.h"
#include "Project_GGF/Public/Items/Bullet/Bullet.h"
#include "GameFramework/Actor.h"
#include "HuntingRifleBullet.generated.h"

UCLASS()
class PROJECT_GGF_API AHuntingRifleBullet : public ABullet
{
	GENERATED_BODY()
	
public:	
	AHuntingRifleBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
