// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPTypes.h"
#include "RPTile.generated.h"

class USceneComponent;
class AStaticMeshActor;
class UStaticMesh;
class ARPBot;
class ARPTurret;
class UBoxComponent;
class ARPDefaultPickup;

UENUM(BlueprintType)
enum class ESpawnTransformType : uint8
{
    ConstructionTransformType_1 = 0,
    ConstructionTransformType_2, 
    ConstructionTransformType_3,
    ConstructionTransformType_4,
    BotTransformType_1,
    BotTransformType_2,
    BotTransformType_3,
    BotTransformType_4,
    TurretTransformType_1,
    TurretTransformType_2,
    TurretTransformType_3,
    TurretTransformType_4
};

UCLASS()
class RETROWAVEPLATFORMER_API ARPTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPTile();

    void SetTileConfiguration(FLevelConfiguration Configuration) { TileConfig = Configuration; };

    void CreateConstruction(UStaticMesh* StaticMesh, FTransform SpawnTransform);

    void SpawnBot(UClass* Class, FTransform SpawnTransform);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile Settings")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constructions")
    UStaticMeshComponent* FloorMesh;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constructions List")
    TArray<UStaticMesh*> ConstructionMeshes;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
    TSubclassOf<ARPBot> MovableEnemie;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
    TSubclassOf<ARPTurret> StaticEnemie;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickups List")
    TArray< TSubclassOf<ARPDefaultPickup> > PickupClasses;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configuration")
    FLevelConfiguration TileConfig {0, 0};

	virtual void BeginPlay() override;

private:
    UPROPERTY()
    TArray<AStaticMeshActor*> Constructions;

    FTransform GetSpawnTransform(ESpawnTransformType TransformType);
};
