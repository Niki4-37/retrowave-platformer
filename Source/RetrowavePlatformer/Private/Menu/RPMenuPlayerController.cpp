// Retrowave platformer game


#include "Menu/RPMenuPlayerController.h"

void ARPMenuPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    //SetInputMode(FInputModeUIOnly());
}
