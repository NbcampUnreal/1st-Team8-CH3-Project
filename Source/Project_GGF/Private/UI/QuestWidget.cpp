#include "UI/QuestWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Gameplay/Quest/QuestData.h"

void UQuestWidget::UpdateQuestUI(const FQuestData& CurrentQuest)
{
	if (!QuestItemList) return;

    if (CurrentQuest.QuestState == EQuestState::Completed)
    {
        QuestItemList->ClearChildren();

        UTextBlock* ClearText = NewObject<UTextBlock>(this);
        if (ClearText)
        {
            ClearText->SetText(FText::FromString(TEXT("오두막으로 돌아가세요")));
            QuestItemList->AddChild(ClearText);
        }
        return;
    }

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
