// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPTypes.h"
#include "RPPlayerHUDWidget.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API URPPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    float GetHealthPercentage() const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
    
};
