#include "Items/Manager/WeaponManager.h"
#include "Items/Weapon/HuntingRifle.h"
#include "Items/Weapon/Shotgun.h"
//#include "../Content/Blueprints/Items/Weapon/BP_HuntingRifle.uasset"


UWeaponManager::UWeaponManager()
{
}

UWeaponManager::~UWeaponManager()
{
}


bool UWeaponManager::Attack()
{
    return false;
}

bool UWeaponManager::Reload()
{
    return false;
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

