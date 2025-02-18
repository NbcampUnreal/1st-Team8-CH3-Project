#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	HuntingRifle UMETA(DisplayName = "HuntingRifle"),
	Shotgun	UMETA(DisplayName = "Shotgun"),
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;		// 사거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Accuracy;		// 정확도
	
	UPROPERTY(EditAnywhere)
	EBulletType BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Damage;		// 공격력

public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
