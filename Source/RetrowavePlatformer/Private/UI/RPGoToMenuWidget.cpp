// Retrowave platformer game


#include "UI/RPGoToMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void URPGoToMenuWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (GoToMainMenuButton)
    {
        GoToMainMenuButton->OnClicked.AddDynamic(this, &URPGoToMenuWidget::OnGoToMainMenu);
    }
}

void URPGoToMenuWidget::OnGoToMainMenu() 
{
    const FName MainMenuLevelName = "MenuLevel";
    UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}
