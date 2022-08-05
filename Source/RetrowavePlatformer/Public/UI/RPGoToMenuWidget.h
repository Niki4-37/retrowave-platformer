// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGoToMenuWidget.generated.h"

class UButton;

UCLASS()
class RETROWAVEPLATFORMER_API URPGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(meta = (BindWidget))
    UButton* GoToMainMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnGoToMainMenu();
};
