#include "AI/Creatures/Animal.h"
#include "AI/AIControllerCustom.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Data/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/Creatures/Bear.h"
#include "AI/Creatures/Boar.h"
#include "AI/Creatures/DeerDoe.h"
#include "AI/Creatures/DeerStag.h"


AAnimal::AAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

    AIControllerClass = AAIControllerCustom::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

    if (IsA<ABear>())
        AnimalType = EAnimalType::Bear;
    else if (IsA<ABoar>())
        AnimalType = EAnimalType::Boar;
    else if (IsA<ADeerDoe>())
        AnimalType = EAnimalType::DeerDoe;
    else if (IsA<ADeerStag>())
        AnimalType = EAnimalType::DeerStag;
}

void AAnimal::BeginPlay()
{
	Super::BeginPlay();

    GenerateRandomLoot();

    AAIControllerCustom* AIController = Cast<AAIControllerCustom>(GetController());
    if (AIController)
    {
        BlackboardComponent = AIController->GetBlackboardComponent();
    }
}

void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAnimal::GenerateRandomLoot()
{
    if (!LootDataTable) return;

    TArray<FName> RowNames = LootDataTable->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        FAnimalLootData* LootData = LootDataTable->FindRow<FAnimalLootData>(RowName, TEXT(""));

        if (LootData && LootData->Animal == AnimalType)
        {
            if (UKismetMathLibrary::RandomFloatInRange(0.f, 1.f) <= LootData->DropChance)
            {
                int32 Quantity = UKismetMathLibrary::RandomIntegerInRange(0, LootData->MaxQuantity);

                FAnimalLoot Loot;
                Loot.ItemID = LootData->ItemID;
                Loot.Quantity = Quantity;

                LootTable.Add(Loot);

                // 나중에 지우기
                UE_LOG(LogTemp, Warning, TEXT("전리품 추가 - 동물: %s, 아이템: %s, 개수: %d"),
                    *UEnum::GetValueAsString(AnimalType),
                    *Loot.ItemID.ToString(),
                    Loot.Quantity);
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("animal 전리품 : %s, 총 %d개"),
        *UEnum::GetValueAsString(AnimalType),
        LootTable.Num());
}


void AAnimal::UpdateAttackState(bool bIsHit)
{
    if (!BlackboardComponent) return;

    if (bIsHit)
    {
        BlackboardComponent->SetValueAsBool(TEXT("bAttacked"), true); 
        BlackboardComponent->SetValueAsVector(TEXT("AttackerLocation"), GetActorLocation());  

        GetWorld()->GetTimerManager().SetTimer(
            AttackResetTimerHandle,
            this,
            &AAnimal::ResetAttackState,
            3.0f, 
            false
        );
    }
}

void AAnimal::ResetAttackState()
{
    if (!BlackboardComponent) return;

    BlackboardComponent->SetValueAsBool(TEXT("bAttacked"), false);
}
