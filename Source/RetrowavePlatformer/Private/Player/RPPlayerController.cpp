// Retrowave platformer game

#include "Player/RPPlayerController.h"
#include "RetrowavePlatformer/RetrowavePlatformerGameModeBase.h"

void ARPPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    
    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        if (const auto RPGameMode = Cast<ARetrowavePlatformerGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            RPGameMode->OnGameStateChanged.AddUObject(this, &ARPPlayerController::OnGameStateChangedn);
        }
    }
}

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

void ARPPlayerController::OnGameStateChangedn(ERPGameState State) 
{
   /* if (State == ERPGameState::InPlay)
    {
        SetInputMode(FInputModeGameOnly());
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
    }*/
}
