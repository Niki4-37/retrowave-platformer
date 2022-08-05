// Retrowave platformer game


#include "Menu/RPMenuGameModeBase.h"
#include "Menu/RPMenuHUD.h"
#include "Menu/RPMenuPlayerController.h"

ARPMenuGameModeBase::ARPMenuGameModeBase() 
{
    HUDClass = ARPMenuHUD::StaticClass();
    PlayerControllerClass = ARPMenuPlayerController::StaticClass();
}
