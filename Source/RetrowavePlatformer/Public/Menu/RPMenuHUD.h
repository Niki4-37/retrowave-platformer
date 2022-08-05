// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPMenuHUD.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API ARPMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu widget")
    TSubclassOf<UUserWidget> MenuWidgetClass;
    
    virtual void BeginPlay() override;
};
