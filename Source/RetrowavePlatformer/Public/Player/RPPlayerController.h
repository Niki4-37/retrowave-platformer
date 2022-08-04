// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPPlayerController.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API ARPPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;
   
private:
    void OnPauseGame();
};
