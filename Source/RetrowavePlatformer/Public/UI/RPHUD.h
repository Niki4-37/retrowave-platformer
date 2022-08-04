// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPTypes.h"
#include "RPHUD.generated.h"


UCLASS()
class RETROWAVEPLATFORMER_API ARPHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> UserWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<ERPGameState, UUserWidget*> UserWidgets;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    void OnGameStateChanged(ERPGameState State);
};
