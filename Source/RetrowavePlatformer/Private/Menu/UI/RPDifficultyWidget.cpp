// Retrowave platformer game


#include "Menu/UI/RPDifficultyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URPDifficultyWidget::SetButtonName(FName Name) 
{
    if (DifficultyTextBlock) 
    {
        DifficultyTextBlock->SetText(FText::FromName(Name));
    }

    ButtonName = Name;
}

void URPDifficultyWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized() ;

    if (DifficultySelectButton)
    {
        DifficultySelectButton->OnClicked.AddDynamic(this, &URPDifficultyWidget::OnDifficultyButtonClicked);
    }
}

void URPDifficultyWidget::OnDifficultyButtonClicked() 
{
    OnDifficultySelected.Broadcast(ButtonName);
}
