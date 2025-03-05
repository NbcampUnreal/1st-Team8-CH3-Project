#include "Project_GGF/Public/Items/Manager/WeaponManager.h"
#include "Project_GGF/Public/Items/Weapon/Weapon.h"
#include "Project_GGF/Public/Items/Weapon/RangedWeapon.h"
#include "Project_GGF/Public/Items/Weapon/MeleeWeapon.h"
#include "Project_GGF/Public/Character/Project_GGFCharacter.h"
#include "AI/NPC/GGFAICharacter.h"
#include "TimerManager.h"
#include "Async/Async.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Items/UtiliyItem/ThrowingItem.h"
#include "Character/GGFCharacterBase.h"
#include "CollisionQueryParams.h"


UWeaponManager::UWeaponManager()
    : CurrentIdx(-1)
    , Weapons{ nullptr, nullptr }
    , WeaponClasses{ 0 }    
    , ThrowingItems{ nullptr, nullptr }
    , ThrowingItemClasses{ 0 }
    , ItemID({ 3, 4 })
{
}

UWeaponManager::~UWeaponManager()
{
}

bool UWeaponManager::Attack()
{
    if (CurrentIdx == -1)
        return false;

    if (CurrentIdx == 0 || CurrentIdx == 1)
    {
        ARangedWeapon* RangedWeapon = Cast<ARangedWeapon>(Weapons[CurrentIdx]);
        if (RangedWeapon)
        {
            if (AGGFCharacterBase* CharacterBase = Cast<AGGFCharacterBase>(Owner))
            {
                FVector CameraLocation = CharacterBase->FollowCamera->GetComponentLocation();
                FRotator CameraRotation = CharacterBase->FollowCamera->GetComponentRotation();

                FVector CameraForward = CameraRotation.Vector(); 

                FVector TraceEnd = CameraLocation + (CameraForward * 10000.0f);
                FHitResult HitResult;
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(CharacterBase); 

                FVector AimPoint = TraceEnd;

                if (Owner->GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams))
                {
                    AimPoint = HitResult.ImpactPoint;
                }

                RangedWeapon->Shot(AimPoint);

            }
            


            return true;
        }
    }

    if (CurrentIdx == 2)
    {
        AThrowingItem* ThrowingItem = Cast<AThrowingItem>(ThrowingItems[0]);
        if (ThrowingItem)
        {
            AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);

            FVector CharacterVelocity = _Character->GetVelocity();

            FVector ThrowDirection = _Character->GetActorForwardVector();

            FVector LaunchVelocity = ThrowDirection * 1000.0f + CharacterVelocity * 0.5f;

            ThrowingItem->Throw(LaunchVelocity);

            ChangeWeapon(0);

            return true;
        }
    }
    else if (CurrentIdx == 3)
    {
        AThrowingItem* ThrowingItem = Cast<AThrowingItem>(ThrowingItems[1]);
        if (ThrowingItem)
        {
            AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);

            FVector CharacterVelocity = _Character->GetVelocity();

            FVector ThrowDirection = _Character->GetActorForwardVector();

            FVector LaunchVelocity = ThrowDirection * 1000.0f + CharacterVelocity * 0.5f;

            ThrowingItem->Throw(LaunchVelocity);

            _Character->GetInventoryObject()->ReturnThrowingItem(ItemID[1]);
            ThrowingItems[1] = nullptr;

            ChangeWeapon(0);

            return true;
        }
    }


    return false;
}

bool UWeaponManager::Reload()
{
    if (CurrentIdx != 0 && CurrentIdx != 1)
        return false;

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
            if (CurrentIdx == 1)
                Weapons[1]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[1]);

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
            if(CurrentIdx == 0)
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
        else if (_Idx == 3)
        {
            if (!_Character->GetInventoryObject()->GetThrowingItem(ItemID[0]))
                return false;

            Detach();

            int32 BoneIndex = _Character->CharacterMesh->GetBoneIndex(LeftHandBone);
            if (BoneIndex != INDEX_NONE)
            {
                FVector SpawnLocation = _Character->CharacterMesh->BoneSpaceTransforms[BoneIndex].GetLocation();
                FQuat Rotation = _Character->CharacterMesh->BoneSpaceTransforms[BoneIndex].GetRotation();

                FTransform SpawnTransform(Rotation, SpawnLocation);

                ThrowingItems[0] = Owner->GetWorld()->SpawnActor<AThrowingItem>(ThrowingItemClasses[0], SpawnTransform);
                ThrowingItems[0]->AttachToComponent(_Character->CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandBone);

                CurrentIdx = 2;
            }

        }
        else if (_Idx == 4)
        {
            if (!_Character->GetInventoryObject()->GetThrowingItem(ItemID[1]))
                return false;

            Detach();

            int32 BoneIndex = _Character->CharacterMesh->GetBoneIndex(LeftHandBone);
            if (BoneIndex != INDEX_NONE)
            {
                FVector SpawnLocation = _Character->CharacterMesh->BoneSpaceTransforms[BoneIndex].GetLocation();
                FQuat Rotation = _Character->CharacterMesh->BoneSpaceTransforms[BoneIndex].GetRotation();

                FTransform SpawnTransform(Rotation, SpawnLocation);

                ThrowingItems[1] = Owner->GetWorld()->SpawnActor<AThrowingItem>(ThrowingItemClasses[1], SpawnTransform);
                ThrowingItems[1]->AttachToComponent(_Character->CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandBone);

                CurrentIdx = 3;
            }
        }

        return true;
    }

    return false;
}

void UWeaponManager::Detach()
{
    AProject_GGFCharacter* _Character = Cast<AProject_GGFCharacter>(Owner);
    if (_Character)
    {
        FName LeftHandBone = _Character->GetHandLSockets();
        FName RightHandBone = _Character->GetHandRSockets();
        TArray<FName> BackBoneName = _Character->GetBackSockets();

        Weapons[0]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[0]);
        Weapons[1]->AttachWeaponToBack(_Character->CharacterMesh, BackBoneName[1]);

        if (CurrentIdx == 2)
        {
            _Character->GetInventoryObject()->ReturnThrowingItem(ItemID[0]);
            ThrowingItems[0]->DestroyItem();
            ThrowingItems[0] = nullptr;
        }
        if (CurrentIdx == 3)
        {
            _Character->GetInventoryObject()->ReturnThrowingItem(ItemID[1]);
            ThrowingItems[1]->DestroyItem();
            ThrowingItems[1] = nullptr;
        }
    }
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
        
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Owner;
        
        Weapons[i] = (Owner->GetWorld()->SpawnActor<AWeapon>(WeaponClasses[i], Location, Rotator, SpawnParams));
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

    // AI
    AGGFAICharacter* AICharacter = Cast<AGGFAICharacter>(Owner);
    if (AICharacter)
    {
        TArray<FName> BackBoneName = AICharacter->GetBackSockets();
        Weapons[0]->AttachWeaponToBack(AICharacter->GetMesh(), BackBoneName[0]);
    }
   
   /* Owner->GetHandSockets();

   
    Owner->GetBackSockets();*/
}


bool UWeaponManager::AttachToBack()
{
    AGGFAICharacter* AICharacter = Cast<AGGFAICharacter>(Owner);
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
    AGGFAICharacter* AICharacter = Cast<AGGFAICharacter>(Owner);
    if (AICharacter)
    {
        FName LeftHandBone = AICharacter->GetHandLSockets();
        FName RightHandBone = AICharacter->GetHandRSockets();

        Weapons[0]->AttachWeaponToHand(AICharacter->GetMesh(), RightHandBone);
        Weapons[0]->AttachWeaponToSocket(AICharacter->GetMesh(), LeftHandBone, "Rifle_L_Socket");
        
        CurrentIdx = 0;
        return true;
    }

    return false;
}

void UWeaponManager::DestroyWeapons()
{
    for (int i = 0; i < Weapons.Num(); i++)
    {
        Weapons[i]->Destroy();
        Weapons[i] = nullptr;
    }
    for (int i = 0; i < ThrowingItems.Num(); i++)
    {
        ThrowingItems[i]->Destroy();
        ThrowingItems[i] = nullptr;
    }
}



