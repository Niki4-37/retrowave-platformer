// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDecalComponent;
class UMaterialInterface;
class URPWeaponComponent;
class URPHealthComponent;

UCLASS()
class RETROWAVEPLATFORMER_API ARPBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARPBaseCharacter();

protected:
    UPROPERTY(EDitDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArm;

    UPROPERTY(EDitDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crosshair")
    UDecalComponent* Crosshair;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crosshair")
    UMaterialInterface* CrosshairMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crosshair")
    FVector CrosshairSize;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    URPWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    URPHealthComponent* HealthComponent;

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable)
    bool IsRunning() const;

private:
    bool bWantsToSprint;

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void WantsToSprint(bool bEnabled);

    float GetMovementDirection() const;

    void SetPlayerRotationToCursor();

    void OnDeath();
};
