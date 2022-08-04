// Retrowave platformer game


#include "AI/RPTurret.h"
#include "Weapon/RPDefaultWeapon.h"
#include "Components/RPHealthComponent.h"
#include "AI/RPAIController.h"
#include "GameFramework/PawnMovementComponent.h"

ARPTurret::ARPTurret()
{
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ARPAIController::StaticClass();

    bUseControllerRotationYaw = true;

    TurretSceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(TurretSceneComponent);

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
    BaseMesh->SetGenerateOverlapEvents(false);
    BaseMesh->SetCollisionProfileName("NoCollision");
    BaseMesh->SetupAttachment(TurretSceneComponent);

    EnduranceComponent = CreateDefaultSubobject<URPHealthComponent>("EnduranceComponent");

}

void ARPTurret::SetWeaponStartFire(bool bIsFiring) 
{
    if (WeaponKit.Num() == 0) return;
    for (const auto& Weapon : WeaponKit)
    {
        Weapon->SetWeaponStartFire(bIsFiring);
    }
}

void ARPTurret::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapons();

    EnduranceComponent->OnDeath.AddUObject(this, &ARPTurret::OnBlasted);
}

void ARPTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARPTurret::SpawnWeapons()
{
    if (!BaseMesh || WeaponClasses.Num() == 0) return;
    uint8 Counter = 0;
    for (const auto& WeaponClass : WeaponClasses)
    {
        ++Counter;
        const auto Weapon = GetWorld()->SpawnActor<ARPDefaultWeapon>(WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(this);
        FString SocketName = "WeaponSocket" + FString::FromInt(Counter);
        Weapon->AttachToComponent(BaseMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(SocketName));

        WeaponKit.Add(Weapon);
    }
}

void ARPTurret::AttachWeaponToSocket(ARPDefaultWeapon* Weapon, USceneComponent* SceneComponent, FName SocketName)
{
    if (!SceneComponent || !Weapon)
    {
        FString DebugText = !SceneComponent ? "No Component to attach" : "" + !Weapon ? "No Weapon to attach" : "";
        UE_LOG(LogTemp, Display, TEXT("%s"), *DebugText);
        return;
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void ARPTurret::OnBlasted() 
{
    for (const auto& Weapon : WeaponKit)
    {
        if (!Weapon) continue;

        Weapon->SetWeaponStartFire(false);
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->SetLifeSpan(10.f);

        if (!Weapon->GetSkeletalMeshComponent()) continue;

        Weapon->GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
        Weapon->GetSkeletalMeshComponent()->SetSimulatePhysics(true);
        //Weapon->GetSkeletalMeshComponent()->AddRadialImpulse(Weapon->GetActorLocation(), 200.f, 200.f, ERadialImpulseFalloff::RIF_Linear);
    }
    WeaponKit.Empty();

    BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BaseMesh->SetSimulatePhysics(true);

}
