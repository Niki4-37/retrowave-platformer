// Retrowave platformer game


#include "Menu/UI/RPMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void URPMenuWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();
    
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &URPMenuWidget::OnGameStart);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &URPMenuWidget::OnQuitGame);
    }
}

void URPMenuWidget::OnGameStart() 
{
    const FName StartUpLevelName = "DevMap";
    UGameplayStatics::OpenLevel(GetWorld(), StartUpLevelName);
}

void URPMenuWidget::OnQuitGame() 
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
