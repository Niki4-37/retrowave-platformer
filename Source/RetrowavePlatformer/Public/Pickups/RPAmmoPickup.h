// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Pickups/RPDefaultPickup.h"
#include "RPAmmoPickup.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API ARPAmmoPickup : public ARPDefaultPickup
{
	GENERATED_BODY()
	
private:
    virtual bool GivePickupToPlayer(APawn* Pawn) override;
};
