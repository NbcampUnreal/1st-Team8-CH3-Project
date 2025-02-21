#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Items/Weapon/Weapon.h"
#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "Project_GGF/Public/Items/Weapon/MeleeWeapon.h"
#include "Character/Project_GGFCharacter.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"


UWeaponManager::UWeaponManager()
    : CurrentIdx(0)
    , Weapons{ nullptr, nullptr }
    , WeaponClasses{ 0 }
{
}

UWeaponManager::~UWeaponManager()
{
}

bool UWeaponManager::Attack()
{
    ARangedWeapon* RangedWeapon = Cast<ARangedWeapon>(Weapons[CurrentIdx]);
    if (Weapons[CurrentIdx] != nullptr)
    {
        RangedWeapon->Shot();
        return true;
    }

    return false;
}

bool UWeaponManager::Reload()
{
    // 원거리 무기인지 확인.
    ARangedWeapon* RangedWeapon = Cast<ARangedWeapon>(Weapons[CurrentIdx]);
    if (Weapons[CurrentIdx] == nullptr)
        return false;

    EBulletType BulletType = RangedWeapon->GetBulletType();

    // Inventory에서 현재총의 탄약개수 확인 후 가져오기.
    // InveTory->GetBulletTotalAmmo(BulletType);
    int TotalAmmo = 30;

    int32 MaxAmmo = RangedWeapon->GetMagazineCapacity();
    int32 CurrentAmmo = RangedWeapon->GetCurrentAmmo();

    if (MaxAmmo == CurrentAmmo)
        return false;

    if (TotalAmmo <= 0)
        return false;

    int32 NeededAmmo = MaxAmmo - CurrentAmmo;

    if (TotalAmmo > NeededAmmo)
    {
        RangedWeapon->Reloading(NeededAmmo);
        TotalAmmo -= NeededAmmo;
    }
    else
    {
        RangedWeapon->Reloading(TotalAmmo);
        TotalAmmo = 0;
    }

    FString BulltetText = FString::Printf(TEXT("Ammo: %d / %d"), RangedWeapon->GetCurrentAmmo(), RangedWeapon->GetMagazineCapacity());
    GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, BulltetText);

    // Inventory에 다시 Bullet개수 수정해놓기.

    return true;
}
bool UWeaponManager::ChangeWeapon(int32 _Idx)
{
    if (_Idx > WeaponClasses.Num())
        return false;

    Weapons[CurrentIdx]->SetActorHiddenInGame(true);

    CurrentIdx = _Idx - 1;

    Weapons[CurrentIdx]->SetActorHiddenInGame(false);

    return true;
}



void UWeaponManager::AddWeapon(AActor* _Actor)
{


    if (_Actor && _Actor->ActorHasTag("Player"))
    {

    }

    // 나중에 태그변경
    else if (_Actor && _Actor->ActorHasTag("Enemy1"))
    {
    }

    // 나중에 태그변경
    else if (_Actor && _Actor->ActorHasTag("Enemy2"))
    {
    }
}

void UWeaponManager::CreateWeapons(ACharacter* _Owner)
{
    if (_Owner == nullptr)
        return;

    Owner = _Owner;

    // Character클래스에서 SkeltalMeshSoket받아와서 부착
    // AttatchToCompnent();

    // Test
    for (int32 i = 0; i < WeaponClasses.Num(); i++)
    {
        AWeapon* Weapon = Cast<AWeapon>(WeaponClasses[i].GetDefaultObject());

        FVector Location = Owner->GetActorLocation();
        FRotator Rotator = Owner->GetActorRotation();
        Weapons[i] = (Owner->GetWorld()->SpawnActor<AWeapon>(WeaponClasses[i], Location, Rotator));


        if (i != 0)
        {
            Weapons[i]->SetActorHiddenInGame(true);
        }

        MaxIdx = i;
    }

    AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);
    if (_Character)
    {
        for (int32 i = 0; i < WeaponClasses.Num(); i++)
        {
            Weapons[i]->AttachWeapon(_Character->GetWeaponSocket());
        }
    }
}



