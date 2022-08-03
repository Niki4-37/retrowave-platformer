// Retrowave platformer game


#include "Components/RPWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/RPDefaultWeapon.h"
#include "Components/SceneComponent.h"

#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(ARPWeaponComponent_LOG, All, All);

URPWeaponComponent::URPWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void URPWeaponComponent::SetWeaponStartFire(bool bIsFiring)
{
    if (!CurrentWeapon || CanDoAction()) return;

    CurrentWeapon->SetWeaponStartFire(bIsFiring);
}

void URPWeaponComponent::ChangeWeapon()
{
    if (CanDoAction()) return;
    
    CurrentWeaponIndex += 1 % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void URPWeaponComponent::TryToReload() 
{
    if (CanDoAction()) return;

    CurrentWeapon->ReloadWeapon();
}


void URPWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    CurrentWeaponIndex = 0;
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void URPWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
    CurrentWeapon = nullptr;
    for (const auto& Weapon : Weapons)
    {
        if (!Weapon) continue;

        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->SetLifeSpan(10.f);

        if (!Weapon->GetSkeletalMeshComponent()) continue;
    
        Weapon->GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
        Weapon->GetSkeletalMeshComponent()->SetSimulatePhysics(true);
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void URPWeaponComponent::SpawnWeapons()
{
    const auto WeaponOwner = Cast<ACharacter>(GetOwner());
    if (!WeaponOwner) return;

    for (const auto& WeaponClass : WeaponClasses)
    {
        const auto Weapon = GetWorld()->SpawnActor<ARPDefaultWeapon>(WeaponClass);
        if (!Weapon) continue;
        
        Weapon->SetOwner(WeaponOwner);
        Weapon->OnReloadStarts.AddUObject(this, &URPWeaponComponent::ReloadStarts);
        Weapon->OnReloadEnds.AddUObject(this, &URPWeaponComponent::ReloadEnds);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, WeaponOwner->GetMesh(), WeaponArmorySocketName);
    }
}

void URPWeaponComponent::AttachWeaponToSocket(ARPDefaultWeapon* Weapon, USceneComponent* SceneComponent, FName SocketName)
{
    if (!SceneComponent || !Weapon)
    {
        FString DebugText = !SceneComponent ? "No Component to attach" : "" + 
                            !Weapon ? "No Weapon to attach" : "";
        UE_LOG(ARPWeaponComponent_LOG, Display, TEXT("%s"), *DebugText);
        return;
    }
    
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void URPWeaponComponent::EquipWeapon(int8 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) return;

    const auto WeaponOwner = Cast<ACharacter>(GetOwner());
    if (!WeaponOwner) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->SetWeaponStartFire(false);
        AttachWeaponToSocket(CurrentWeapon, WeaponOwner->GetMesh(), WeaponArmorySocketName);
    }
    
    CurrentWeapon = Weapons[WeaponIndex];

    //Play Equip animation replace AttachWepon to animation notify
    AttachWeaponToSocket(CurrentWeapon, WeaponOwner->GetMesh(), WeaponSocketName);
}

bool URPWeaponComponent::CanDoAction()
{
    return bEquipInProgress && bReloadInProgress;
}

void URPWeaponComponent::ReloadStarts() 
{
    bReloadInProgress = true;
    PlayAnimation(ReloadAnimation);
}

void URPWeaponComponent::ReloadEnds() 
{
    bReloadInProgress = false;
}

void URPWeaponComponent::PlayAnimation(UAnimMontage* Animation) 
{
    const auto WeaponOwner = Cast<ACharacter>(GetOwner());
    if (!WeaponOwner || !Animation) return;

    UE_LOG(ARPWeaponComponent_LOG, Display, TEXT("%s"), *UKismetSystemLibrary::GetDisplayName(Animation));
    WeaponOwner->PlayAnimMontage(Animation);
}



