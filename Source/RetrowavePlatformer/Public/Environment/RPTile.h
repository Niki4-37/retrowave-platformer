// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPTile.generated.h"

class USceneComponent;
class AStaticMeshActor;
class UStaticMesh;
class ARPBot;
class ARPTurret;

UENUM(BlueprintType)
enum class ESpawnTransformType : uint8
{
    TransformType_1 = 0,
    TransformType_2, 
    TransformType_3,
    TransformType_4,
    TransformType_5,
    TransformType_6,
    TransformType_7
};

UCLASS()
class RETROWAVEPLATFORMER_API ARPTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPTile();

    void CreateConstruction(UStaticMesh* StaticMesh, FTransform SpawnTransform);

    void SpawnBot(UClass* Class, FTransform SpawnTransform);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile Settings")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constructions")
    UStaticMeshComponent* FloorMesh;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constructions")
    TArray<UStaticMesh*> ConstructionMeshes;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
    TSubclassOf<ARPBot> MovableEnemie;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
    TSubclassOf<ARPTurret> StaticEnemie;


	virtual void BeginPlay() override;

private:
    UPROPERTY()
    TArray<AStaticMeshActor*> Constructions;

    FTransform GetSpawnTransform(ESpawnTransformType TransformType);
};
