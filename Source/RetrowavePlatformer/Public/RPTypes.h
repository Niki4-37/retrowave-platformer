#pragma once

#include "RPTypes.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class ARPBot;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bIsEndless"))
    int32 Magazines;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsEndless;
};

// USTRUCT(BlueprintType)
// struct FImpactData
//{
//     GENERATED_USTRUCT_BODY()
//
//     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
//     class USoundCue* ImpactSound;
//
//     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//     UNiagaraSystem* ImpactEffect;
//
//     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//     UMaterialInterface* DecalMaterial;
//
//     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//     FVector DecalSize = FVector(20.0f);
//
//     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//     float DecalLifeTime = 5.0f;
//
//     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//     float DecalFadeOutTime = 0.7f;
// };

UENUM(BlueprintType)
enum class ERPGameState : uint8
{
    WaitingToStart = 0,
    InPlay,
    Paused,
    GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, ERPGameState);