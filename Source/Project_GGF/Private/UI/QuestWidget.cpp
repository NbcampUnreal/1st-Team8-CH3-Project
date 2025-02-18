#include "UI/QuestWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Gameplay/Quest/QuestData.h"

void UQuestWidget::UpdateQuestUI(const FQuestData& CurrentQuest)
{
	if (!QuestItemList) return;

    for (const auto& Item : CurrentQuest.TargetItems)
    {
        int32 CollectedCount = CurrentQuest.CurrentItems.Contains(Item.Key) ? CurrentQuest.CurrentItems[Item.Key] : 0;
        int32 TargetCount = Item.Value;

        UTextBlock* ItemText = NewObject<UTextBlock>(this); 
        if (ItemText)
        {
            ItemText->SetText(FText::FromString(FString::Printf(TEXT("%s: %d / %d"), *Item.Key, CollectedCount, TargetCount)));
            QuestItemList->AddChild(ItemText); 
        }
    }
}
