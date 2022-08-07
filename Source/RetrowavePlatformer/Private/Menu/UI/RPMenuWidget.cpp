// Retrowave platformer game


#include "Menu/UI/RPMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "RPGameInstance.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/RPDifficultyWidget.h"

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

    InitDifficultyButtons();
}

void URPMenuWidget::OnGameStart() 
{
    const FName StartUpLevelName = "GameLevel";
    UGameplayStatics::OpenLevel(GetWorld(), StartUpLevelName);
}

void URPMenuWidget::OnQuitGame() 
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void URPMenuWidget::InitDifficultyButtons() 
{
    if (!GetWorld()) return;
    
    auto GameInstance = GetWorld()->GetGameInstance<URPGameInstance>();

    if (!GameInstance) return;
    
    if (!DifficultyButtonsBox) return;
    DifficultyButtonsBox->ClearChildren();

    UE_LOG(LogTemp, Display, TEXT("Difficulty buttons in list: %i"), GameInstance->GetGameDificultyList().Num());
    int Index = 0;    

    for (const auto& GameDifficultName : GameInstance->GetGameDificultyList())
    {
        ++Index;
        const auto DifficultyWidget = CreateWidget<URPDifficultyWidget>(GetWorld(), DifficultyWidgetClass);
        if (!DifficultyWidget) continue;

        DifficultyWidget->SetButtonName(GameDifficultName);
        DifficultyWidget->OnDifficultySelected.AddUObject(this, &URPMenuWidget::OnDifficultySelected);

        DifficultyButtonsBox->AddChild(DifficultyWidget);
        DifficultyButtons.Add(DifficultyWidget);
    }
    UE_LOG(LogTemp, Display, TEXT("Difficulty buttons: %i"), Index);

}

void URPMenuWidget::OnDifficultySelected(FName Name) 
{
    if (!GetWorld()) return;

    auto GameInstance = GetWorld()->GetGameInstance<URPGameInstance>();
    if (!GameInstance) return;

    GameInstance->SetGameDifficulty(Name);
}
