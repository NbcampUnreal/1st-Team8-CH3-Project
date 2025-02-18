#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PROJECT_GGF_API ABullet : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComp;

public:	
	ABullet();
	ABullet(FString _Mesh);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
