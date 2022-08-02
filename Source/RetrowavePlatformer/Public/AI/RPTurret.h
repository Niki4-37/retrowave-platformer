// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RPTurret.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API ARPTurret : public APawn
{
	GENERATED_BODY()

public:
	ARPTurret();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
