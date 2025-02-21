#include "AI/Creatures/DeerDoe.h"
#include "Kismet/GameplayStatics.h"

void ADeerDoe::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1초마다 정보 갱신
    static float UpdateInterval = 1.0f;
    static float Time = 0.0f;

    Time += DeltaTime;

    if (Time >= UpdateInterval)
    {
        FindNearbyDeer();
        Time = 0.0f;
    }
}

// 주변 사슴들 찾기
void ADeerDoe::FindNearbyDeer()
{
    NearbyDeer.Empty();

    TArray<AActor*> FoundDeer;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeerDoe::StaticClass(), FoundDeer);

    for (AActor* Deer : FoundDeer)
    {
        if (Deer != this && FVector::Dist(Deer->GetActorLocation(), GetActorLocation()) < HerdRadius)
        {
            NearbyDeer.Add(Deer);
        }
    }
}

// 주변 사슴들의 평균 위치를 계산
FVector ADeerDoe::CalculateAveragePos()
{
    if (NearbyDeer.Num() == 0)
    {
        return GetActorLocation();
    }

    FVector CenterPoint = GetActorLocation();
    for (AActor* Deer : NearbyDeer)
    {
        CenterPoint += Deer->GetActorLocation();
    }

    return CenterPoint / NearbyDeer.Num();
}

