// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPDefaultWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraSystem;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 Magazines;
};

//USTRUCT(BlueprintType)
//struct FImpactData
//{
//    GENERATED_USTRUCT_BODY()
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
//    class USoundCue* ImpactSound;
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//    UNiagaraSystem* ImpactEffect;
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//    UMaterialInterface* DecalMaterial;
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//    FVector DecalSize = FVector(20.0f);
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//    float DecalLifeTime = 5.0f;
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
//    float DecalFadeOutTime = 0.7f;
//};

DECLARE_MULTICAST_DELEGATE(FOnReloadStartsSignature);
DECLARE_MULTICAST_DELEGATE(FOnReloadEndsSignature);

UCLASS()
class RETROWAVEPLATFORMER_API ARPDefaultWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPDefaultWeapon();

    FOnReloadStartsSignature OnReloadStarts;
    FOnReloadEndsSignature  OnReloadEnds;

    void SetWeaponStartFire(bool bIsFiring);

    void ReloadWeapon();
    
    //for UserWidget
    UFUNCTION(BlueprintCallable)
    float ReturnRemainingReloadTimePercent() const;
    UFUNCTION(BlueprintCallable)
    float ReturnRemainingLoadCartrigeTimePercent() const;
    
    virtual void Tick(float DeltaTime) override;
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
    USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
    float WeaponDamage {10.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
    float TraceDistance {2000.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
    float RateOfFire {0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
    bool bIsAutomatic {true};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
    float ReloadingTime{ 1.5f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
    float WeaponSpreadDegrees {7.f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings")
    FAmmoData DefaultAmmo { 10, 5 };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings")
    FName MuzzleSocketName { "MuzzleSocket" };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName EjectionSocketName { "EjectPort" };
    
    /*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* EjectEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    FImpactData ImpactData;*/

	virtual void BeginPlay() override;

    void MakeShot();

    void ReduceAmmo();

    void MakeHitScan();

private:
    FAmmoData CurrentAmmo;
    bool bIsPlayerWantsFire {false};
    bool bIsCartrigeLoaded{ true };
    FTimerHandle AtomaticFireTimer;

    bool bReloadInProgress {false};
    FTimerHandle ReloadingTimer;

    void LoadCartrigeAndShot();

    void DealDamage(const FHitResult& HitResult);

    bool CanFire();

    void ReloadFinished();

    //void CreateFXImpactEffect(const FHitResult& Hit);
};
