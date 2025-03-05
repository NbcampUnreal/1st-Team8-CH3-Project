#include "AI/Creatures/Animal.h"
#include "AI/GGFAIController.h"
#include "Character/Data/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/Creatures/Bear.h"
#include "AI/Creatures/Boar.h"
#include "AI/Creatures/DeerDoe.h"
#include "AI/Creatures/DeerStag.h"
#include "Components/BoxComponent.h"

AAnimal::AAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

    AIControllerClass = AGGFAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    
    if (IsA<ABear>())
        AnimalType = EAnimalType::Bear;
    else if (IsA<ABoar>())
        AnimalType = EAnimalType::Boar;
    else if (IsA<ADeerDoe>())
        AnimalType = EAnimalType::DeerDoe;
    else if (IsA<ADeerStag>())
        AnimalType = EAnimalType::DeerStag;

    AttackHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitbox"));
    AttackHitbox->SetupAttachment(GetMesh(), TEXT("head"));

    AttackHitbox->IgnoreActorWhenMoving(this, true);
}

void AAnimal::BeginPlay()
{
	Super::BeginPlay();

    GenerateRandomLoot();

    if (AttackHitbox)
    {
        AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        AttackHitbox->OnComponentBeginOverlap.AddDynamic(this, &AAnimal::OnAttackHitboxOverlap);
    }
}

void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAnimal::OnDie()
{
    Super::OnDie();

    SetAttackHitboxActive(false);
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

void AAnimal::OnAttackHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    AGGFCharacterBase* OtherCharacter = Cast<AGGFCharacterBase>(OtherActor);
    if (!OtherCharacter) return;

    UHealthComponent* TargetHealthComp = OtherCharacter->FindComponentByClass<UHealthComponent>();
    if (TargetHealthComp)
    {
        TargetHealthComp->TakeDamage(this, EAttackType::Melee, 0.0f, 10);
        UE_LOG(LogTemp, Warning, TEXT("%s이(가) %s를 머리로 공격!"), *GetName(), *OtherCharacter->GetName());
    }
}

void AAnimal::SetAttackHitboxActive(bool bActive)
{
    if (AttackHitbox)
    {
        AttackHitbox->SetCollisionEnabled(bActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
        if (bActive)
        {
            // 자기 자신을 계속 무시
            AttackHitbox->IgnoreActorWhenMoving(this, true);
        }
    }
}

