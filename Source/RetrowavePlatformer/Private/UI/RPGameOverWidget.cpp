// Retrowave platformer game


#include "UI/RPGameOverWidget.h"
#include "GameFrameWork/GameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void URPGameOverWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (RestartLevelButton)
    {
        RestartLevelButton->OnClicked.AddDynamic(this, &URPGameOverWidget::OnRestartLevel);
    }
}

void URPGameOverWidget::OnRestartLevel()
{
    const auto CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
