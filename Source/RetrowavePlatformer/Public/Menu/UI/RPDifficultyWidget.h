// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPTypes.h"
#include "RPDifficultyWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class RETROWAVEPLATFORMER_API URPDifficultyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    FOnDifficultySelectedSignature OnDifficultySelected;

    void SetButtonName(FName Name);

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* DifficultySelectButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DifficultyTextBlock;
	
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnDifficultyButtonClicked();

    FName ButtonName;
};
