// Retrowave platformer game


#include "Player/RPPlayerController.h"
#include "GameFramework/GameModeBase.h"

void ARPPlayerController::SetupInputComponent() 
{
    Super::SetupInputComponent();
    if (!InputComponent) return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ARPPlayerController::OnPauseGame);
}

void ARPPlayerController::OnPauseGame() 
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}
