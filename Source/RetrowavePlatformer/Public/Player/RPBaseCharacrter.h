// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPBaseCharacrter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDecalComponent;
class UMaterialInterface;


UCLASS()
class RETROWAVEPLATFORMER_API ARPBaseCharacrter : public ACharacter
{
	GENERATED_BODY()

public:
	ARPBaseCharacrter();

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
};
