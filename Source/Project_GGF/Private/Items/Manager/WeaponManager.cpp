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
    // ���Ÿ� �������� Ȯ��.
    ARangedWeapon* RangedWeapon = Cast<ARangedWeapon>(Weapons[CurrentIdx]);
    if (Weapons[CurrentIdx] == nullptr)
        return false;

    EBulletType BulletType= RangedWeapon->GetBulletType();

    // Inventory���� �������� ź�ళ�� Ȯ�� �� ��������.
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
    
    // Inventory�� �ٽ� Bullet���� �����س���.

    return true;
}

void UWeaponManager::AddWeapon(AActor* _Actor)
{
    // Player�� ��� 
    if (_Actor && _Actor->ActorHasTag("Player"))
    {
    }

    // ���߿� �±׺���
    else if (_Actor && _Actor->ActorHasTag("Enemy1"))
    {
    }

    // ���߿� �±׺���
    else if (_Actor && _Actor->ActorHasTag("Enemy2"))
    {
    }
}

