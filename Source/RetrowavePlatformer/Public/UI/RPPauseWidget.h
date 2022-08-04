// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPPauseWidget.generated.h"

class UButton;

UCLASS()
class RETROWAVEPLATFORMER_API URPPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(meta = (BindWidget))
    UButton* UndoPauseButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnUndoPause();
};
