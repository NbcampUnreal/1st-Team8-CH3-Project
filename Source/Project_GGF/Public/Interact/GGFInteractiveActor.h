#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GGFInteractiveActor.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class PROJECT_GGF_API AGGFInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGGFInteractiveActor();

protected:
	virtual void BeginPlay() override;

public:	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<USceneComponent> Root;

    // 트리거 박스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<UBoxComponent> WidgetTriggerBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<UWidgetComponent> NotifyInteractionWidget;

    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    bool bIsPlayerNearby = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	AActor* InteractableActor;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ShowInteractionWidget(bool bShow);
	
    UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    virtual void InteractionKeyPressed(AActor* Actor);
};
