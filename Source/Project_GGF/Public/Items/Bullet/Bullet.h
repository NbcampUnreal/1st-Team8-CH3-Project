#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UCapsuleComponent;

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
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,  Category = "Component")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;		// 사거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Accuracy;		// 정확도
	
	UPROPERTY(EditAnywhere)
	EBulletType BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Damage;		// 공격력

	FTimerHandle DestroyTimer;

	FVector InitialLocation;

protected:
	UFUNCTION()
	virtual void OnBulletOverlap(
		UPrimitiveComponent* _overlapComp,
		AActor* _otherActor,
		UPrimitiveComponent* _otherComp,
		int32 _otherBodyIndex,
		bool _bFromSweep,
		const FHitResult& _sweepResult
	);
	UFUNCTION()
	virtual void OnBulletEndOverlap(
		UPrimitiveComponent* _overlapComp,
		AActor* _otherActor,
		UPrimitiveComponent* _otherComp,
		int32 _otherBodyIndex
	);

	UFUNCTION()
	void OnProjectileStop(const FHitResult& _ImpacResult);
	void BulletDestroy();

public:
	void SetProjectileVelocity(FVector _Velocity);
	float GetProjectileInitialSpeed();

public:
	ABullet();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
