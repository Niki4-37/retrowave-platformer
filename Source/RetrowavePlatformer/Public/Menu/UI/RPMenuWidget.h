// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPMenuWidget.generated.h"

class UButton;

UCLASS()
class RETROWAVEPLATFORMER_API URPMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnGameStart();

    UFUNCTION()
    void OnQuitGame();
};
