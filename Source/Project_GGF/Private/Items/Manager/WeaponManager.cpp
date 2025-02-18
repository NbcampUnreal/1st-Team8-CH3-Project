#include "Items/Manager/WeaponManager.h"
#include "Items/Weapon/Weapon.h"
#include "Items/Weapon/RangedWeapon.h"
#include "Items/Weapon/MeleeWeapon.h"


UWeaponManager::UWeaponManager()
    : CurrentIdx(0)
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

    EBulletType BulletType= RangedWeapon->GetBulletType();

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
    
    // Inventory에 다시 Bullet개수 수정해놓기.

    return true;
}

void UWeaponManager::AddWeapon(AActor* _Actor)
{
    // Player일 경우 
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

