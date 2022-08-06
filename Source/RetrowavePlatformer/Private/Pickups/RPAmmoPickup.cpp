// Retrowave platformer game

#include "Pickups/RPAmmoPickup.h"
#include "Components/RPHealthComponent.h"
#include "Components/RPWeaponComponent.h"

bool ARPAmmoPickup::GivePickupToPlayer(APawn* Pawn)
{
    const auto HealthComponent = Pawn->FindComponentByClass<URPHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = Pawn->FindComponentByClass<URPWeaponComponent>();
    if (!WeaponComponent ) return false;
    WeaponComponent->TryToAddAmmo();
    
    return true;
}
