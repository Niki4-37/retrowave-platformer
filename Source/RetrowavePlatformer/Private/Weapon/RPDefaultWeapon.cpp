// Retrowave platformer game

#include "Weapon/RPDefaultWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "Components/DecalComponent.h"

DEFINE_LOG_CATEGORY_STATIC(RPDefaultWeapon_LOG, All, All);

ARPDefaultWeapon::ARPDefaultWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
    SetRootComponent(SkeletalMesh);
}

void ARPDefaultWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(SkeletalMesh);
    CurrentAmmo = DefaultAmmo;
}

void ARPDefaultWeapon::SetWeaponStartFire(bool bIsFiring)
{
    bIsPlayerWantsFire = bIsFiring;

    if (!CanFire()) return;

    if (bIsPlayerWantsFire)
    {
        MakeShot();
    }

    if (!GetWorld()->GetTimerManager().IsTimerActive(AtomaticFireTimer) && bIsPlayerWantsFire)
    {
        GetWorld()->GetTimerManager().SetTimer(AtomaticFireTimer,  //
            this,                                                  //
            &ARPDefaultWeapon::LoadCartrigeAndShot,                //
            RateOfFire,                                            //
            true);
    }
}

void ARPDefaultWeapon::ReloadWeapon()
{
    if (bReloadInProgress || CurrentAmmo.Bullets == DefaultAmmo.Bullets || AmmoEmpty())
    {
        UE_LOG(RPDefaultWeapon_LOG, Display, TEXT("Empty ammo"));
        return;
    }

    bReloadInProgress = true;
    UE_LOG(RPDefaultWeapon_LOG, Display, TEXT("Start reload"));

    GetWorld()->GetTimerManager().SetTimer(ReloadingTimer,  //
        this,                                               //
        &ARPDefaultWeapon::ReloadFinished,                  //
        ReloadingTime,                                      //
        false);

    OnReloadStarts.Broadcast();
}

 void ARPDefaultWeapon::CreateFXImpactEffect(const FHitResult& Hit)
{
    FImpactData ImpactData;

    if (Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMat))
        {
            ImpactData = ImpactDataMap[PhysMat];
        }
        else
        {
            return;
        }
    }

    if (ImpactData.ImpactEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.ImpactEffect, Hit.ImpactPoint,
        Hit.ImpactNormal.Rotation());
    }

    if (ImpactData.DecalMaterial)
    {
        auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.DecalMaterial, ImpactData.DecalSize,
        Hit.ImpactPoint, Hit.ImpactNormal.Rotation()); if (DecalComponent)
        {
            DecalComponent->SetFadeOut(ImpactData.DecalLifeTime, ImpactData.DecalFadeOutTime);
        }
    }

    if (ImpactData.ImpactSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactData.ImpactSound, Hit.ImpactPoint);
    }
}

void ARPDefaultWeapon::TryToAddAmmo() 
{
    if (CurrentAmmo.bIsEndless) return;

    CurrentAmmo.Magazines = DefaultAmmo.Magazines;
}

void ARPDefaultWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARPDefaultWeapon::MakeShot()
{
    if (!bIsCartrigeLoaded) return;

    CreateFXWeaponEffect();

    MakeHitScan();

    ReduceAmmo();

    bIsCartrigeLoaded = false;
}

void ARPDefaultWeapon::ReduceAmmo()
{
    --CurrentAmmo.Bullets;

    if (MagazineEmpty())
    {
        UE_LOG(RPDefaultWeapon_LOG, Display, TEXT("Magazines is empty"));

        GetWorld()->GetTimerManager().PauseTimer(AtomaticFireTimer);

        ReloadWeapon();
    }
}

void ARPDefaultWeapon::LoadCartrigeAndShot()
{
    bIsCartrigeLoaded = true;

    if (bIsPlayerWantsFire && bIsAutomatic && CanFire())
    {
        MakeShot();
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(AtomaticFireTimer);
    }
}

void ARPDefaultWeapon::MakeHitScan()
{
    // SkeletalMesh checks at BeginPlay()
    const FVector TraceStart = SkeletalMesh->GetSocketLocation(MuzzleSocketName);
    const auto OwnerDirection = GetOwner() ? GetOwner()->GetActorForwardVector() : FVector::ZeroVector;
    const FVector ShootDirection = FMath::VRandCone(OwnerDirection, FMath::DegreesToRadians(WeaponSpreadDegrees));
    FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;

    FHitResult HitResult;

    FCollisionQueryParams QuerryParams;
    QuerryParams.AddIgnoredActor(GetOwner());
    QuerryParams.bReturnPhysicalMaterial = true;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QuerryParams);

    if (HitResult.bBlockingHit)
    {
        TraceEnd = HitResult.ImpactPoint;
        
        CreateFXImpactEffect(HitResult);

        DealDamage(HitResult);
    }
    
    SpawnTraceFX(TraceStart, TraceEnd);
}

void ARPDefaultWeapon::DealDamage(const FHitResult& HitResult)
{
    if (const auto DamagedActor = HitResult.GetActor())
    {
        UGameplayStatics::ApplyDamage(DamagedActor, WeaponDamage, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
    }
}

bool ARPDefaultWeapon::CanFire()
{
    if (bReloadInProgress || CurrentAmmo.Bullets <= 0 || !bIsCartrigeLoaded) return false;

    return true;
}

bool ARPDefaultWeapon::MagazineEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

bool ARPDefaultWeapon::AmmoEmpty() const
{
    return !CurrentAmmo.bIsEndless && CurrentAmmo.Magazines == 0 && MagazineEmpty();
}

void ARPDefaultWeapon::ReloadFinished()
{
    bReloadInProgress = false;
    bIsCartrigeLoaded = true;
    GetWorld()->GetTimerManager().UnPauseTimer(AtomaticFireTimer);
    GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    --CurrentAmmo.Magazines;
    OnReloadEnds.Broadcast();
    UE_LOG(RPDefaultWeapon_LOG, Display, TEXT("Reload ends"));
}

void ARPDefaultWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!TraceFX) return;

    const auto TraceFXEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),                                 //
        TraceFX,                                                //
        TraceStart);

    if (!TraceFXEffect) return;
    TraceFXEffect->SetNiagaraVariableVec3("TraceFXEnd", TraceEnd);
}

void ARPDefaultWeapon::CreateFXWeaponEffect() 
{
    if (MuzzleEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAttached(
                MuzzleEffect, 
                SkeletalMesh, 
                MuzzleSocketName,
                FVector::ZeroVector, 
                FRotator::ZeroRotator, 
                EAttachLocation::SnapToTarget, 
                true);
}

    if (SleeveEjectEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), 
            SleeveEjectEffect, 
            SkeletalMesh->GetSocketLocation(EjectionSocketName),
            SkeletalMesh->GetSocketRotation(EjectionSocketName));
    }

     if (ShootingSound)
     {
         UGameplayStatics::SpawnSoundAtLocation(GetWorld(),                //
             ShootingSound,  //
             SkeletalMesh->GetSocketLocation(MuzzleSocketName));
     }
 }
