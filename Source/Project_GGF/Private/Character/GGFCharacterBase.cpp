#include "Character/GGFCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interact/TreasureChestInteractiveActor.h"
#include "AI/Creatures/Animal.h"
#include "Character/Data/HitDeadComponent.h"
#include "Character/Data/HealthComponent.h"
#include "Character/Data/RespawnComponent.h"
#include "Character/Data/StaminaComponent.h"
#include "Character/Data/NoiseComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/GGFGameMode.h"

AGGFCharacterBase::AGGFCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
    
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArmComp);

   
    WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
    HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    HitDeadComp = CreateDefaultSubobject<UHitDeadComponent>(TEXT("HitDeadComponent"));
    RespawnComp = CreateDefaultSubobject<URespawnComponent>(TEXT("RespawnComponent"));
    StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
    NoiseComp = CreateDefaultSubobject<UNoiseComponent>(TEXT("NoiseComponent"));
    
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    
    MaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
    SpeedBoostDuration = 5.0f;
    SpeedBoostMultiplier = 1.5f;

    QuietSpeedMultiplier = 0.5;
    QuietSpeed = GetCharacterMovement()->MaxWalkSpeed * QuietSpeedMultiplier;
    
    SitSpeedMultiplier = 0.4;
    SitSpeed = GetCharacterMovement()->MaxWalkSpeed * SitSpeedMultiplier;
    
    SprintSpeedMultiplier = 2.0f;
    SprintSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintSpeedMultiplier;

    CurrentWeapon = nullptr;
    
    HandSockets = { TEXT("L_HandSocket"), TEXT("R_HandSocket") };
    BackSockets = { TEXT("L_BackSocket"), TEXT("R_BackSocket") };
}

void AGGFCharacterBase::BeginPlay()
{
	Super::BeginPlay();

    
}

void AGGFCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGGFCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGGFCharacterBase::StartSprint()
{
    if (bIsSitting == true)
    {
        return;
    }
    else
    {
        UpdateSprint();
    }
   
}
void AGGFCharacterBase::UpdateSprint()
{
    if (!StaminaComp || StaminaComp->GetStamina() <= 0 || !GetCharacterMovement())
    {
        StopSprint();
        return;
    }

    if (!bIsSprinting)
    {
        bIsSprinting = true;
        if (GetCharacterMovement())
        {
            GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
        }

        if (StaminaComp)
        {
            StaminaComp->StopStaminaRecovery();

            GetWorld()->GetTimerManager().SetTimer(
                SprintStaminaHandle,
                [this]() 
                {
                    if (StaminaComp)
                    { StaminaComp->UseStamina();}
                },
                0.5f,
                true
            );
        }

        if (NoiseComp)
        {
            if (GetWorld()->GetTimerManager().IsTimerActive(NoiseComp->NoiseTimerHandle))
            {
                NoiseComp->StopNoiseTimer();
            }
            NoiseComp->StartNoiseTimer(100.0f, 750.0f);
        }
    }
}
void AGGFCharacterBase::StopSprint()
{
        bIsSprinting = false;

        if (GetCharacterMovement())
        {
            GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
        }

        GetWorld()->GetTimerManager().ClearTimer(SprintStaminaHandle);

        StaminaComp->StartStaminaRecovery();

        NoiseComp->NoiseDelay = 0.25f;
        NoiseComp->StopNoiseTimer();
}

void AGGFCharacterBase::Reload()
{
        if (bIsArmed == false){ return; }
        else
        {
            UpdateReload();
        }
        
}
void AGGFCharacterBase::UpdateReload()
{
        bIsReload = true;
        if (WeaponManager)
        {
            WeaponManager->Reload();
			
            GetWorld()->GetTimerManager().SetTimer(
                ReloadTimer,
                this,
                &AGGFCharacterBase::FinishReload,
                1.0f,
                false);
        }		
}
void AGGFCharacterBase::FinishReload()
{
        bIsReload = false;
        GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
}
void AGGFCharacterBase::ToggleSit()
{
        bIsSitting = !bIsSitting;

        if (GetCharacterMovement())
        {
            GetCharacterMovement()->MaxWalkSpeed = bIsSitting ? SitSpeed : MaxSpeed;
            NoiseComp->NoiseIntensity = 150.0f;
            NoiseComp->NoiseRadius = 10.0f;
        }
}
void AGGFCharacterBase::StartFire()
{
        if (bIsArmed == true)
        {
            UpdateFire();
        }
        
}
void AGGFCharacterBase::UpdateFire()
{
        bIsFiring = true;
        WeaponManager->Attack();
        
        NoiseComp->NoiseIntensity = 200.0f;
        NoiseComp->NoiseRadius = 1500.0f;
        NoiseComp->GenerateNoiseTimer();

        GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AGGFCharacterBase::StopFire, 0.1f, false);
}
void AGGFCharacterBase::StopFire()
{
        bIsFiring = false;
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
}
    
void AGGFCharacterBase::StartQuiet()
{
        UpdateQuiet();
}
void AGGFCharacterBase::UpdateQuiet()
{
    if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = QuietSpeed;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(NoiseComp->NoiseTimerHandle))
	{
		NoiseComp->StopNoiseTimer();
	}

	NoiseComp->NoiseIntensity = 10.0f;
	NoiseComp->NoiseRadius = 200.0f;
	NoiseComp->StartNoiseTimer(NoiseComp->NoiseIntensity, NoiseComp->NoiseRadius);

	StaminaComp->StopStaminaRecovery();
        
    
}
void AGGFCharacterBase::StopQuiet()
{
        bIsQuiet = false;

        if (GetCharacterMovement())
        {
            GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
        }

        NoiseComp->StopNoiseTimer();
}
void AGGFCharacterBase::Interact()
{
        UpdateInteract();
}
void AGGFCharacterBase::UpdateInteract()
{
        bIsInteract = true;
        if (FocusedHidePlace)
        {
            FocusedHidePlace->InteractionKeyPressed(this); 
        }

 /*       if (InteractableActor)
        {
            InteractableActor->InteractionKeyPressed(this);
        }*/
}
void AGGFCharacterBase::EndInteract()
{
        bIsInteract = false;
	
        if (FocusedHidePlace)
        {
            FocusedHidePlace->ShowInteractionWidget(false); 
            FocusedHidePlace = nullptr; 
        }
}


void AGGFCharacterBase::AddLootToInventory(const TArray<FAnimalLoot>& LootItems)
{
    if(InventoryObjectInstance)
        InventoryObjectInstance->AddLootItem(LootItems);
}

TArray<FAnimalLoot> AGGFCharacterBase::GetInventoryLoot() const
{
    return Inventory;
}

void AGGFCharacterBase::OnDie()
{
    if (IsDead)
        return;

    IsDead = true;

    HitDeadComp->PlayDeadMontage();
    
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
    }
    
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    
    
    FVector DeathLocation = GetActorLocation();
   
    AGGFGameMode* GameMode = Cast<AGGFGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->HandleLootDrop(this, HealthComp->LastAttacker, DeathLocation);
    }
}

void AGGFCharacterBase::OnHit(AActor* Attacker)
{
    if (IsDead)
        return;
    
    HitDeadComp->PlayHitMontage();
}

void AGGFCharacterBase::ActivateSpeedBoost()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed *= SpeedBoostMultiplier;
        GetWorld()->GetTimerManager().SetTimer(
        SpeedBoostTimerHandle,
        this,
        &AGGFCharacterBase::ResetSpeedBoost,
        SpeedBoostDuration,
        false 
        );
    }
}


void AGGFCharacterBase::ResetSpeedBoost()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed /= SpeedBoostMultiplier;
    }
}

void AGGFCharacterBase::PerformInteractionTrace()
    {
    FVector Start = FollowCamera->GetComponentLocation();
    FVector ForwardVector = FollowCamera->GetForwardVector();
    FVector End = ((ForwardVector * 500.0f) + Start);
    
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
    {
        if (HitResult.GetActor() && HitResult.GetActor()->IsA(AHidePlace::StaticClass()))
        {
			
            if (!FocusedHidePlace || (GetWorld()->GetTimeSeconds() - LastInteractionTime) > InteractionCooldownTime)
            {
                FocusedHidePlace = Cast<AHidePlace>(HitResult.GetActor());
                FocusedHidePlace->ShowInteractionWidget(true);
            }
        }
            else
            {
                if (FocusedHidePlace)
                {
                    FocusedHidePlace->ShowInteractionWidget(false);
                }
                FocusedHidePlace = nullptr;
            }
        }
        else
        {
            if (FocusedHidePlace)
            {
                FocusedHidePlace->ShowInteractionWidget(false);
            }
            FocusedHidePlace = nullptr;
        }
    }


void AGGFCharacterBase::PerformInteractionCheck()
    {
        PerformInteractionTrace();
        InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
    }


