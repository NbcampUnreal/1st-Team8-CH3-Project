#include "Gameplay/Quest/QuestManager.h"
#include "Gameplay/GGFGameMode.h"
#include "Gameplay/Quest/QuestItemData.h"
#include "Blueprint/UserWidget.h"
#include "UI/QuestWidget.h"

AQuestManager::AQuestManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AQuestManager::BeginPlay()
{
    Super::BeginPlay();

    if (!ItemTable) return;

    GenerateRandomQuest();

    if (QuestWidgetClass)
    {
        QuestWidget = CreateWidget<UQuestWidget>(GetWorld(), QuestWidgetClass);
        if (QuestWidget)
        {
            QuestWidget->AddToViewport();
            QuestWidget->UpdateQuestUI(CurrentQuest);
        }
    }

    GetWorldTimerManager().SetTimer(QuestCheckTimerHandle, this, &AQuestManager::CheckQuestStatus, 1.0f, true);
}

void AQuestManager::GenerateRandomQuest()
{
    TArray<FQuestItemData*> AllItems;
    ItemTable->GetAllRows<FQuestItemData>(TEXT(""), AllItems);

    if (AllItems.Num() == 0) return;

    TMap<EItemRarity, TArray<FQuestItemData*>> RarityMap;
    for (FQuestItemData* Item : AllItems)
    {
        RarityMap.FindOrAdd(Item->Rarity).Add(Item);
    }

    int32 NumItems = 5; 
    int32 MaxCnt = 3; 
    while (CurrentQuest.TargetItems.Num() < NumItems)
    {
        float RandValue = FMath::FRand();  
        EItemRarity SelectedRarity;

        if (RandValue < CommonProbability)
            SelectedRarity = EItemRarity::Common;
        else if (RandValue < CommonProbability + RareProbability)
            SelectedRarity = EItemRarity::Rare;
        else
            SelectedRarity = EItemRarity::Legend;

        FQuestItemData* SelectedItem = RarityMap[SelectedRarity][FMath::RandRange(0, RarityMap[SelectedRarity].Num() - 1)];

        if (CurrentQuest.TargetItems.Contains(SelectedItem->ItemName)) 
            continue;

        int32 ItemCount = FMath::RandRange(1, MaxCnt);
        CurrentQuest.TargetItems.Add(SelectedItem->ItemName, ItemCount);
        CurrentQuest.CurrentItems.Add(SelectedItem->ItemName, 0);

        UE_LOG(LogTemp, Warning, TEXT("퀘스트 목표: %s %d개"), *SelectedItem->ItemName, ItemCount);
    }
 }

void AQuestManager::UpdateQuestProgress(FString ItemName, int32 Amount)
{
    if (CurrentQuest.QuestState != EQuestState::InProgress) return;

    if (CurrentQuest.TargetItems.Contains(ItemName))
    {
        int32 CurrentCount = CurrentQuest.CurrentItems[ItemName];
        int32 TargetCount = CurrentQuest.TargetItems[ItemName];

        if (CurrentCount >= TargetCount) return;

        int32 NewCount = FMath::Min(CurrentCount + Amount, TargetCount);
        CurrentQuest.CurrentItems[ItemName] = NewCount;

        UE_LOG(LogTemp, Warning, TEXT("%s 획득! (현재 %d/%d)"), *ItemName, NewCount, TargetCount);

        if (QuestWidget)
        {
            QuestWidget->UpdateQuestUI(CurrentQuest);
        }

        CheckQuestStatus();
    }
}

void AQuestManager::CheckQuestStatus()
{
    AGGFGameMode* GameMode = Cast<AGGFGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    float TimeRatio = GameMode->GetGameTimeRatio();

    // 퀘스트 실패
    if (TimeRatio >= 1.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("퀘스트 실패! 제한 시간 초과."));

        /*
        if (GameOverWidgetClass)
        {
            GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
            if (GameOverWidget)
            {
                GameOverWidget->AddToViewport();
            }
        }*/
        GameMode->GameOver();

        GetWorldTimerManager().ClearTimer(QuestCheckTimerHandle);

        return;
    }

    // 퀘스트 성공
    bool bAllCollected = true;
    for (const auto& Item : CurrentQuest.TargetItems)
    {
        if (CurrentQuest.CurrentItems[Item.Key] < Item.Value)
        {
            bAllCollected = false;
            break;
        }
    }

    if (bAllCollected)
    {
        CurrentQuest.QuestState = EQuestState::Completed;

        UE_LOG(LogTemp, Warning, TEXT("퀘스트 완료!"));

        if (QuestWidget)
        {
            QuestWidget->UpdateQuestUI(CurrentQuest);
        }
    }
}


