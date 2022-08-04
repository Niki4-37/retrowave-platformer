// Retrowave platformer game


#include "UI/RPHUD.h"
#include "Blueprint/UserWidget.h"
#include "RetrowavePlatformer/RetrowavePlatformerGameModeBase.h"

void ARPHUD::BeginPlay() 
{
    Super::BeginPlay();

    const auto RPUserWidget = CreateWidget<UUserWidget>(GetWorld(), UserWidgetClass);
    if (RPUserWidget)
    {
        RPUserWidget->AddToViewport();
    }

    if (GetWorld())
    {
        const auto RPGameMode = Cast<ARetrowavePlatformerGameModeBase>(GetWorld()->GetAuthGameMode());
        if (RPGameMode)
        {
            RPGameMode->OnGameStateChanged.AddUObject(this, &ARPHUD::OnGameStateChanged);
        }
    }

}

void ARPHUD::OnGameStateChanged(ERPGameState State) 
{

}
