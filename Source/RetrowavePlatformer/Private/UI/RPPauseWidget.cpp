// Retrowave platformer game


#include "UI/RPPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"


void URPPauseWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (UndoPauseButton)
    {
        UndoPauseButton->OnClicked.AddDynamic(this, &URPPauseWidget::OnUndoPause);
    }
}

void URPPauseWidget::OnUndoPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    
    GetWorld()->GetAuthGameMode()->ClearPause();
}
