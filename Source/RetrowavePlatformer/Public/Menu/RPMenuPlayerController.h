// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPMenuPlayerController.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API ARPMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
};
