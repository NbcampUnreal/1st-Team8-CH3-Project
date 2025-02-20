#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestData.h"
#include "QuestManager.generated.h"

UCLASS()
class PROJECT_GGF_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AQuestManager();

	UFUNCTION(BlueprintCallable)
	void UpdateQuestProgress(FString ItemName, int32 Amount);  

	UFUNCTION(BlueprintCallable)
	void CheckQuestStatus(); 

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	FQuestData CurrentQuest;  
	UPROPERTY(EditAnywhere, Category = "Quest")
	UDataTable* ItemTable;  

	FTimerHandle QuestCheckTimerHandle;

	void GenerateRandomQuest();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UQuestWidget> QuestWidgetClass;
	UPROPERTY()
	UQuestWidget* QuestWidget;

	/*
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UPROPERTY()
	UUserWidget* GameOverWidget;*/

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestText;
};
