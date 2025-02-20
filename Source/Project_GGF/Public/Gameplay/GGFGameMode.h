#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GGFGameMode.generated.h"

UCLASS()
class PROJECT_GGF_API AGGFGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGGFGameMode();

    float GetGameTimeRatio() const;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void GameOver();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    float TotalGameTime = 900.0f; 
    float CurrentTime = 0.0f;

    FTimerHandle GameTimeHandle;
    void UpdateGameTime();
};
