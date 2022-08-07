// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RPTurret.generated.h"

class ARPDefaultWeapon;
class URPHealthComponent;
class UBehaviorTree;

UCLASS()
class RETROWAVEPLATFORMER_API ARPTurret : public APawn
{
	GENERATED_BODY()

public:
	ARPTurret();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Behavior")
    UBehaviorTree* BehaviorTreeAsset;

    void SetWeaponStartFire(bool bIsFiring);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Settings")
    USceneComponent* TurretSceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Settings")
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TArray<TSubclassOf<ARPDefaultWeapon> > WeaponClasses;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Endurance")
    URPHealthComponent* EnduranceComponent;

    virtual void BeginPlay() override;

public:
    void SpawnWeapons();

    void AttachWeaponToSocket(ARPDefaultWeapon* Weapon, USceneComponent* SceneComponent, FName SocketName);

private:
    TArray<ARPDefaultWeapon*> WeaponKit;

    

    void OnBlasted();
};
