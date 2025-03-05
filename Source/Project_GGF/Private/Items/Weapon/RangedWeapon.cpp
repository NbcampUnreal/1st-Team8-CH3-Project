#include "Items/Weapon/RangedWeapon.h"
#include "TimerManager.h"  
#include "Items/Bullet/Bullet.h"
#include "Items/Bullet/TestBullet.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


ARangedWeapon::ARangedWeapon()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneComp);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftHandSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandSceneComp"));
	LeftHandSceneComp->SetupAttachment(SceneComp);

	MuzzleSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSceneComp"));
	MuzzleSceneComp->SetupAttachment(SceneComp);

	WeaponSockets = { TEXT("Rifle_L_Socket"), TEXT("Rifle_R_Socket") };

	// FString MuzzleNiagaraPath = "/Game/GGF/VFX/Niagara/Muzzle/NE_VFX_Muzzle_Rifle.NE_VFX_Muzzle_Rifle";
	// MuzzleNiagaraSys =  Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *MuzzleNiagaraPath));
	// MuzzleNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleNiagaraSys, MuzzleSceneComp, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::KeepRelativeOffset, true);

	// FX
	// 나이아가라 컴포넌트 생성 및 설정
	MuzzleNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleNiagaraComp"));
	MuzzleNiagaraComp->SetupAttachment(MuzzleSceneComp);
	MuzzleNiagaraComp->SetAutoActivate(false); // 자동 활성화 비활성화

	// 오디오 컴포넌트 생성 및 설정
	MuzzleAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MuzzleAudioComp"));
	MuzzleAudioComp->SetupAttachment(MuzzleSceneComp);
	MuzzleAudioComp->SetAutoActivate(false); // 자동 활성화 비활성화


}

ARangedWeapon::~ARangedWeapon()
{
}

void ARangedWeapon::PlaySound()
{
	if (MuzzleAudioComp)
	{
		MuzzleAudioComp->Activate(true);
	}
}

void ARangedWeapon::PlayVFX()
{
	if (MuzzleNiagaraComp)
	{
		MuzzleNiagaraComp->Activate(true);
	}
}

bool ARangedWeapon::Shot(FVector AimPoint)
{
	return true;
}

bool ARangedWeapon::Reloading(int32 _NeededAmmo)
{
	if (_NeededAmmo <= 0)
		return 0;

	CurrentAmmo += _NeededAmmo;

	GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &ARangedWeapon::EndReloading, ReloadingDelay, false);
	bIsReloading = true;

	return true;
}

void ARangedWeapon::EndFireDelay()
{
	bIsFireDelay = false;
}

void ARangedWeapon::EndReloading()
{
	bIsReloading = false;
}

