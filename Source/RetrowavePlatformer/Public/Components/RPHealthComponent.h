// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RETROWAVEPLATFORMER_API URPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    URPHealthComponent();

    FOnDeathSignature OnDeath;

    void SetHealth(float NewHealth);

    void TryToHeal();

    bool IsFullHealth() const { return Health == MaxHealth; };

    UFUNCTION(BlueprintCallable)
    float GetHealthPercentage() const { return Health / MaxHealth; };
    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    float MaxHealth{100.f};

    virtual void BeginPlay() override;

private:
    float Health;

    UFUNCTION()
    void TakeAnyDamage(                 //
        AActor* DamagedActor,           //
        float Damage,                   //
        const UDamageType* DamageType,  //
        AController* InstigatedBy,      //
        AActor* DamageCauser);
};
