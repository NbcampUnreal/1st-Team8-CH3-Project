#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Items/Weapon/Weapon.h"
#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "Project_GGF/Public/Items/Weapon/MeleeWeapon.h"
#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "AI/AICharacter.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"


UWeaponManager::UWeaponManager()
    : CurrentIdx(-1)
    , Weapons{ nullptr, nullptr }
    , WeaponClasses{ 0 }
{
}

UWeaponManager::~UWeaponManager()
{
}

bool UWeaponManager::Attack()
{
    if (CurrentIdx == -1)
        return false;

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
    if (CurrentIdx == _Idx - 1)
        return false;

    AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);
    if (_Character)
    {
        //TArray<USceneComponent*> HandSceneComp = _Character->GetHandSockets();
        //TArray<USceneComponent*> BackSceneComp = _Character->GetBackSockets();
        TArray<FName> HandBoneName= _Character->GetHandSockets();
        TArray<FName> BackBoneName= _Character->GetBackSockets();
        
        if (_Idx == 0)
        {
            // GetBackSocket()[0] 왼쪽자리
            Weapons[0]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[0]);

            // GetBackSocket()[1] 오른쪽자리
            Weapons[1]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[1]);
            CurrentIdx = -1;
        }
        else if (_Idx == 1)
        {
            // GetHandSocket()
            Weapons[0]->AttachWeaponToHand(_Character->CharacterMesh, HandBoneName);

            // GetBackSocket()[1] 오른쪽자리
            Weapons[1]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[1]);
            CurrentIdx = 0;
        }
        else if (_Idx == 2)
        {
            // GetBackSocket()[0] 왼쪽자리
            Weapons[0]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[0]);

            // GetHandSocket()
            Weapons[1]->AttachWeaponToHand(_Character->CharacterMesh, HandBoneName);
            CurrentIdx = 1;
        }

        return true;

    }

    return false;
    

    

    
    //if (_Idx > WeaponClasses.Num())
    //    return false;

    //Weapons[CurrentIdx]->SetActorHiddenInGame(true);

    //CurrentIdx = _Idx - 1;

    //Weapons[CurrentIdx]->SetActorHiddenInGame(false);

    //return true;
}





void UWeaponManager::CreateWeapons(ACharacter* _Owner)
{
    if (_Owner == nullptr)
        return;

    Owner = _Owner;

    Weapons.SetNum(WeaponClasses.Num());

    for (int32 i = 0; i < WeaponClasses.Num(); i++)
    {
        FVector Location = Owner->GetActorLocation();
        FRotator Rotator = Owner->GetActorRotation();
        Weapons[i] = (Owner->GetWorld()->SpawnActor<AWeapon>(WeaponClasses[i], Location, Rotator));
        MaxIdx = i;
    }

    AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);
    if (_Character)
    {
        //TArray<USceneComponent*> SceneComp = _Character->GetBackSockets();
        TArray<FName> BoneName = _Character->GetBackSockets();
        TArray<FName> HandBoneName = _Character->GetHandSockets();

        for (int32 i = 0; i < WeaponClasses.Num(); i++)
        {
            Weapons[i]->AttachWeaponToBack(_Character->CharacterMesh, BoneName[i]);
        }

        //Weapons[0]->AttachWeaponToHand(_Character->CharacterMesh, HandBoneName);
        //CurrentIdx = 0;
    }

   

    // AI도 마찬가지
    // ***
    //
    // ***




    //Weapons.SetNum(WeaponClasses.Num());

    //for (int32 i = 0; i < WeaponClasses.Num(); i++)
    //{
    //    AWeapon* Weapon = Cast<AWeapon>(WeaponClasses[i].GetDefaultObject());

    //    FVector Location = Owner->GetActorLocation();
    //    FRotator Rotator = Owner->GetActorRotation();
    //    Weapons[i] = (Owner->GetWorld()->SpawnActor<AWeapon>(WeaponClasses[i], Location, Rotator));


    //    if (i != 0)
    //    {
    //        Weapons[i]->SetActorHiddenInGame(true);
    //    }

    //    MaxIdx = i;
    //}

    //AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);
    //if (_Character)
    //{
    //    for (int32 i = 0; i < WeaponClasses.Num(); i++)
    //    {
    //        //Weapons[i]->AttachWeaponToBack(_Character->GetWeaponSocket());
    //    }
    //}



   /* Owner->GetHandSockets();

   
    Owner->GetBackSockets();*/
}

bool UWeaponManager::AttachToBack()
{
    if (CurrentIdx == -1)
        return false;

    AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);
    if (_Character)
    {
        //TArray<USceneComponent*> BackSceneComp = _Character->GetBackSockets();
        TArray<FName> BackSceneName = _Character->GetBackSockets();
        Weapons[0]->AttachWeaponToBack(_Character->CharacterMesh, BackSceneName[0]);
        Weapons[1]->AttachWeaponToBack(_Character->CharacterMesh, BackSceneName[1]);
        CurrentIdx = -1;
        return true;
    }
    // AI
    // =====
    //Weapons[0]->AttachWeaponToBack(Owner->GetWeaponBackSocket());
    // =====
    //CurrentIdx = -1;
    return false;
}

bool UWeaponManager::AttachToHand()
{
    //if (CurrentIdx == 0)
    //    return false;

    //Weapons[0]->AttachWeaponToHand(Owner->GetWeaponHandSocket());
    CurrentIdx = 0;
    return true;
}



