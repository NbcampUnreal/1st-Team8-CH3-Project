#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

UCLASS()
class PROJECT_GGF_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateQuestUI(const FQuestData& CurrentQuest);

protected:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* QuestItemList;
};
