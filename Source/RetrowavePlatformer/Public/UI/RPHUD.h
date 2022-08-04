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

    virtual void BeginPlay() override;

private:
    void OnGameStateChanged(ERPGameState State);
};
