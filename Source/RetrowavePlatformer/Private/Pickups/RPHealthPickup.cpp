// Retrowave platformer game


#include "Pickups/RPHealthPickup.h"
#include "Components/RPHealthComponent.h"

bool ARPHealthPickup::GivePickupToPlayer(APawn* Pawn)
{
    const auto HealthComponent = Pawn->FindComponentByClass<URPHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead() || HealthComponent->IsFullHealth()) return false;

    HealthComponent->TryToHeal();
    return true;
}
