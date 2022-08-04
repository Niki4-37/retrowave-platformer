// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGameOverWidget.generated.h"

class UButton;

UCLASS()
class RETROWAVEPLATFORMER_API URPGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* RestartLevelButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnRestartLevel();
};
