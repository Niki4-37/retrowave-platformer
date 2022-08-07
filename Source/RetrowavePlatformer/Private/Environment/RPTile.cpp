// Retrowave platformer game

#include "Environment/RPTile.h"
#include "Engine/StaticMeshActor.h"
#include "AI/RPBot.h"
#include "AI/RPTurret.h"
#include "Components/BoxComponent.h"
#include "Pickups/RPDefaultPickup.h"
#include "Pickups/RPAmmoPickup.h"
#include "Pickups/RPHealthPickup.h"

ARPTile::ARPTile()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>("FloorMesh");
    FloorMesh->SetupAttachment(GetRootComponent());
    FloorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    FloorMesh->SetCollisionProfileName("BlockAll");
}

void ARPTile::CreateConstruction(UStaticMesh* StaticMesh, FTransform SpawnTransform)
{
    if (!StaticMesh) return;

    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    auto Construction = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnTransform, Param);

    if (!Construction || !Construction->GetStaticMeshComponent()) return;

    Construction->GetStaticMeshComponent()->SetCollisionProfileName("BlockAll");
    Construction->GetStaticMeshComponent()->Mobility = EComponentMobility::Static;
    Construction->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
}

void ARPTile::SpawnBot(UClass* Class, FTransform SpawnTransform)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const auto NewBot = GetWorld()->SpawnActor<APawn>(Class, SpawnTransform, SpawnParams);
    if (!NewBot) return;
}

void ARPTile::BeginPlay()
{
    Super::BeginPlay();
    check(GetWorld());

    uint8 ConstructionsNum = static_cast<uint8>(FMath::RandHelper(3) + 1);
    for (uint8 Index = 0; Index < ConstructionsNum; ++Index)
    {
        const FTransform ConstructionSpawnTransform = GetSpawnTransform(static_cast<ESpawnTransformType>(Index));
        CreateConstruction(ConstructionMeshes[0], ConstructionSpawnTransform);
    }

    for (uint8 BotIndex = 0; BotIndex < TileConfig.BotsNum; ++BotIndex)
    {
        const FTransform BotSpawnTransform = GetSpawnTransform(static_cast<ESpawnTransformType>(FMath::RandHelper(3) + 4));
        SpawnBot(MovableEnemie, BotSpawnTransform);
    }

    for (uint8 BotIndex = 0; BotIndex < TileConfig.TurretsNum; ++BotIndex)
    {
        const FTransform TurretSpawnTransform = GetSpawnTransform(static_cast<ESpawnTransformType>(FMath::RandHelper(3) + 8));
        SpawnBot(StaticEnemie, TurretSpawnTransform);
    }
    
    if (PickupClasses.Num() > 0)
    {
        const auto RandIndex = FMath::RandHelper(PickupClasses.Num());
        UE_LOG(LogTemp, Display, TEXT("Bonus index: %i"), RandIndex);
        const auto PickupClass = PickupClasses[RandIndex];
        const FTransform PickupTransform{FTransform(FRotator::ZeroRotator, GetActorLocation() + FVector(0.f, 0.f, 100.f))};
        const auto Pickup = GetWorld()->SpawnActor<ARPDefaultPickup>(PickupClass, PickupTransform);
    }

}

FTransform ARPTile::GetSpawnTransform(ESpawnTransformType TransformType)
{
    FTransform SpawnTransform = FTransform();

    switch (TransformType)
    {
        case ESpawnTransformType::ConstructionTransformType_1:
            SpawnTransform =
                FTransform(FRotator(0.f, FMath::RandRange(0.f, 180.f), 0.f), GetActorLocation() + FVector(+700.f, +300.f, 150.f));
            break;
        case ESpawnTransformType::ConstructionTransformType_2:
            SpawnTransform =
                FTransform(FRotator(0.f, FMath::RandRange(0.f, 180.f), 0.f), GetActorLocation() + FVector(-700.f, -300.f, 100.f));
            break;
        case ESpawnTransformType::ConstructionTransformType_3:
            SpawnTransform =
                FTransform(FRotator(0.f, FMath::RandRange(0.f, 180.f), 0.f), GetActorLocation() + FVector(+300.f, -700.f, 100.f));
            break;
        case ESpawnTransformType::ConstructionTransformType_4:
            SpawnTransform =
                FTransform(FRotator(0.f, FMath::RandRange(0.f, 180.f), 0.f), GetActorLocation() + FVector(-300.f, +700.f, 100.f));
            break;
        case ESpawnTransformType::BotTransformType_1:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(+300.f, +300.f, 100.f));
            break;
        case ESpawnTransformType::BotTransformType_2:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(+300.f, -300.f, 100.f));
            break;
        case ESpawnTransformType::BotTransformType_3:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(-300.f, -300.f, 150.f));
            break;
        case ESpawnTransformType::BotTransformType_4:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(-300.f, +300.f, 150.f));
            break;
        case ESpawnTransformType::TurretTransformType_1:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(+300.f, +700.f, 150.f));
            break;
        case ESpawnTransformType::TurretTransformType_2:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(-700.f, 300.f, 150.f));
            break;
        case ESpawnTransformType::TurretTransformType_3:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(-300.f, -700.f, 150.f));
            break;
        case ESpawnTransformType::TurretTransformType_4:
            SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation() + FVector(+700.f, -300.f, 150.f));
            break;
        default: SpawnTransform = FTransform(); break;
    }

    return SpawnTransform;
}
