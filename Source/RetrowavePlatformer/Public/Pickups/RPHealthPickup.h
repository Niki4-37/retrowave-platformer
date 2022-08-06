// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Pickups/RPDefaultPickup.h"
#include "RPHealthPickup.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API ARPHealthPickup : public ARPDefaultPickup
{
	GENERATED_BODY()
	
private:
    virtual bool GivePickupToPlayer(APawn* Pawn);
};
