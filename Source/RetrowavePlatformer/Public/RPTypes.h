#pragma once

#include "RPTypes.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class ARPBot;

USTRUCT(BlueprintType)
struct FTileData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constructions")
    UStaticMeshComponent* FloorMesh;

};