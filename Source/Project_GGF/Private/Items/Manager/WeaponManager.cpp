#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Items/Weapon/Weapon.h"
#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "Project_GGF/Public/Items/Weapon/MeleeWeapon.h"
#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "AI/AICharacter.h"
#include "TimerManager.h"
#include "Async/Async.h"
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

//Weapon_Left
bool UWeaponManager::ChangeWeapon(int32 _Idx)
{
    if (CurrentIdx == _Idx - 1)
        return false;

    AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);
    if (_Character)
    {
        FName LeftHandBone = _Character->GetHandLSockets();
        FName RightHandBone = _Character->GetHandRSockets();
        TArray<FName> BackBoneName = _Character->GetBackSockets();

        if (_Idx == 0) 
        {
            Weapons[0]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[0]);
            Weapons[1]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[1]);
            CurrentIdx = -1;
        }
        else if (_Idx == 1) 
        {
            
            Weapons[0]->AttachWeaponToHand(_Character->CharacterMesh, RightHandBone);
            Weapons[0]->AttachWeaponToSocket(_Character->CharacterMesh, LeftHandBone, "Rifle_L_Socket");

           
            if (Weapons[0] != nullptr)
            {
                Weapons[0]->HideWeapon();
            }
            _Character->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, _Character, LeftHandBone, RightHandBone]()
              {
                  if (Weapons[0] != nullptr)
                  {
                      Weapons[0]->ShowWeapon();
                  }

                  
                  Weapons[1]->AttachWeaponToBack(_Character->CharacterMesh, _Character->GetBackSockets()[1]);

                  CurrentIdx = 0;
              }, 0.3f, false);
            
        }
        else if (_Idx == 2)
        {
            Weapons[0]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[0]);

            
            Weapons[1]->AttachWeaponToHand(_Character->CharacterMesh, RightHandBone);
            Weapons[1]->AttachWeaponToSocket(_Character->CharacterMesh, LeftHandBone, "Rifle_L_Socket"); 
            CurrentIdx = 1;
            
          
            if (Weapons[1] != nullptr)
            {
                Weapons[1]->HideWeapon();
            }
            
            _Character->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, _Character, LeftHandBone, RightHandBone]()
                {
                    if (Weapons[1] != nullptr)
                    {
                        Weapons[1]->ShowWeapon();
                    }

                    
                    Weapons[0]->AttachWeaponToBack(_Character->CharacterMesh, _Character->GetBackSockets()[0]);

                    CurrentIdx = 1;
                }, 0.3f, false);
            
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

   
   /* Owner->GetHandSockets();

   
    Owner->GetBackSockets();*/
}

bool UWeaponManager::AttachToBack()
{
    AAICharacter* AICharacter = Cast<AAICharacter>(Owner);
    if (AICharacter)
    {
        TArray<FName> BackBoneName = AICharacter->GetBackSockets();
        Weapons[0]->AttachWeaponToBack(AICharacter->GetMesh(), BackBoneName[0]);
        return true;
    }

    return false;
}

bool UWeaponManager::AttachToHand()
{
    AAICharacter* AICharacter = Cast<AAICharacter>(Owner);
    if (AICharacter)
    {
        TArray<FName> HandBoneName = AICharacter->GetHandSockets();
        Weapons[0]->AttachWeaponToBack(AICharacter->GetMesh(), HandBoneName[0]);
        return true;
    }

    return false;
}



