// Retrowave platformer game


#include "Menu/RPMenuHUD.h"
#include "Blueprint/UserWidget.h"

void ARPMenuHUD::BeginPlay() 
{
    Super::BeginPlay();
    check(GetWorld());

    if (MenuWidgetClass)
    {
        const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
        }
    }
}
