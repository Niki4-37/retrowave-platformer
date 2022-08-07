// Retrowave platformer game

#include "UI/RPHUD.h"
#include "Blueprint/UserWidget.h"
#include "RetrowavePlatformer/RetrowavePlatformerGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(RPHUD_LOG, All, All);

void ARPHUD::BeginPlay()
{
    Super::BeginPlay();
    check(GetWorld());

    if (UserWidgetClass)
    {
        UserWidgets.Add(ERPGameState::InPlay, CreateWidget<UUserWidget>(GetWorld(), UserWidgetClass));
    }
    if (PauseWidgetClass)
    {
        UserWidgets.Add(ERPGameState::Paused, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    }
    if (GameOverWidgetClass)
    {
        UserWidgets.Add(ERPGameState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));
    }

    for (const auto& GameWidgetPair : UserWidgets)
    {
        if (const auto DefiniteWidget = GameWidgetPair.Value)
        {
            DefiniteWidget->AddToViewport();
            DefiniteWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    const auto RPGameMode = Cast<ARetrowavePlatformerGameModeBase>(GetWorld()->GetAuthGameMode());
    if (RPGameMode)
    {
        RPGameMode->OnGameStateChanged.AddUObject(this, &ARPHUD::OnGameStateChanged);
    }
}

void ARPHUD::OnGameStateChanged(ERPGameState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (UserWidgets.Contains(State))
    {
        CurrentWidget = UserWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }

    UE_LOG(RPHUD_LOG, Display, TEXT("Current game state: %s"), *UEnum::GetValueAsString(State));
}
