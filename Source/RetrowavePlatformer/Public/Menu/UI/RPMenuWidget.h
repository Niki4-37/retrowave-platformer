// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPTypes.h"
#include "RPMenuWidget.generated.h"

class UButton;
class URPGameInstance;
class UHorizontalBox;
class URPDifficultyWidget;

UCLASS()
class RETROWAVEPLATFORMER_API URPMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* DifficultyButtonsBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Difficulty Settings")
    TSubclassOf<URPDifficultyWidget> DifficultyWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UUserWidget*> DifficultyButtons;

    UFUNCTION()
    void OnGameStart();

    UFUNCTION()
    void OnQuitGame();

    void InitDifficultyButtons();

    void OnDifficultySelected(FName Name);
    
};
