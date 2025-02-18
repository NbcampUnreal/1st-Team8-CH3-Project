#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;
class USphereComponent;


UCLASS()
class PROJECT_GGF_API ABullet : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere)
	USphereComponent* CapsuleComp;

public:	
	ABullet();
	ABullet(FString _Mesh);
	ABullet(FString _Mesh, FString _Material);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
