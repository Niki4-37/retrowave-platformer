// Retrowave platformer game


#include "Environment/RPTile.h"
#include "Engine/StaticMeshActor.h"
#include "AI/RPBot.h"

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
    Construction->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    Construction->GetStaticMeshComponent()->Mobility = EComponentMobility::Movable;
    Construction->GetStaticMeshComponent()->SetSimulatePhysics(true);
    //Construction->SetPivotOffset() to customize position depends static mesh height
    Construction->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
    
    //Constructions.Add(Construction);
}

void ARPTile::SpawnBot(FTransform SpawnTransform) 
{
    /*FString TextReady = bIsSpawnOccupied ? "true" : "false";
    UE_LOG(LogTemp, Display, TEXT("Actor: %s, SpawnOccupied: %s"), *GetDebugName(this), *TextReady);*/

    // if (!GetWorld() || bIsSpawnOccupied) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const auto NewBot = GetWorld()->SpawnActor<APawn>(MovableEnemie, SpawnTransform, SpawnParams);
    //UE_LOG(LogTemp, Display, TEXT("Actor spawned: %s"), *NewBot->GetName());
    if (!NewBot) return;

    /*if (!NewPawn->Controller)
    {
        NewPawn->SpawnDefaultController();
    }

    const auto AIController = Cast<AAIController>(NewPawn->Controller);
    if (AIController)
    {
        const auto AICharacter = Cast<AEPAICharacter>(AIController->GetCharacter());
        if (!AICharacter) return;

        AIController->RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }*/
}

void ARPTile::BeginPlay()
{
	Super::BeginPlay();
    check(GetWorld());
    
    for (uint8 Index = 0; Index < 3; ++Index)
    {
        const FTransform SpawnTransform = GetSpawnTransform(static_cast<ESpawnTransformType>(Index));
        CreateConstruction(ConstructionMeshes[0], SpawnTransform);
    }

    const FTransform EnemySpawnTransform = GetSpawnTransform(static_cast<ESpawnTransformType>(FMath::RandHelper(3) + 3));
    SpawnBot(EnemySpawnTransform);
}

FTransform ARPTile::GetSpawnTransform(ESpawnTransformType TransformType)
{
    FTransform SpawnTransform = FTransform();

    switch (TransformType)
    {
        case ESpawnTransformType::TransformType_1: 
            SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 90.f), 0.f), GetActorLocation() + FVector(100.f, 200.f, 100.f));
            break;
        case ESpawnTransformType::TransformType_2: 
            SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 90.f), 0.f), GetActorLocation() + FVector(800.f, -200.f, 100.f)); 
            break;
        case ESpawnTransformType::TransformType_3: 
            SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 90.f), 0.f), GetActorLocation() + FVector(-200.f, -200.f, 100.f)); 
            break;
        case ESpawnTransformType::TransformType_4:
            SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 90.f), 0.f), GetActorLocation() + FVector(-450.f, 300.f, 100.f));
            break;
        case ESpawnTransformType::TransformType_5:
            SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 90.f), 0.f), GetActorLocation() + FVector(250.f, -500.f, 100.f));
            break;
        case ESpawnTransformType::TransformType_6:
            SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 90.f), 0.f), GetActorLocation() + FVector(400.f, 500.f, 100.f));
            break;
        default: 
            SpawnTransform = FTransform();
            break;
    }

    return SpawnTransform;
}


