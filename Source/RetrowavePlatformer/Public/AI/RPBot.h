// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPTypes.h"
#include "RPBot.generated.h"

class URPHealthComponent;

UCLASS()
class RETROWAVEPLATFORMER_API ARPBot : public ACharacter
{
	GENERATED_BODY()

public:
	ARPBot();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    URPHealthComponent* HealthComponent;

	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void OnDeath();
};
