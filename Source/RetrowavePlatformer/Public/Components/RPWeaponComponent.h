// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPWeaponComponent.generated.h"

class ARPDefaultWeapon;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RETROWAVEPLATFORMER_API URPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPWeaponComponent();

    void SetWeaponStartFire(bool bIsFiring);

    void ChangeWeapon();

    void TryToReload();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<ARPDefaultWeapon> > WeaponClasses;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
    FName WeaponSocketName = "WeaponSocket";
 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Settings")
    FName WeaponArmorySocketName = "WeaponArmorySocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* EquipAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* ReloadAnimation;

	virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    bool bEquipInProgress {false};
    bool bReloadInProgress {false};

    UPROPERTY()
    ARPDefaultWeapon* CurrentWeapon {nullptr};

    UPROPERTY()
    TArray<ARPDefaultWeapon*> Weapons;

    uint8 CurrentWeaponIndex {0};
    
    void SpawnWeapons();

    void AttachWeaponToSocket(ARPDefaultWeapon* Weapon, USceneComponent* SceneComponent, FName SocketName);

    void EquipWeapon(int8 WeaponIndex);

    bool CanDoAction();

    void ReloadStarts();
    void ReloadEnds();

    void PlayAnimation(UAnimMontage* Animation);
};
