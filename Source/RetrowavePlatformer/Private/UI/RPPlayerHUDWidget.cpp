// Retrowave platformer game


#include "UI/RPPlayerHUDWidget.h"
#include "Components/RPHealthComponent.h"
#include "Components/RPWeaponComponent.h"

float URPPlayerHUDWidget::GetHealthPercentage() const
{
    if (!GetOwningPlayerPawn()) return 0.f;
    const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<URPHealthComponent>();
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercentage();
}

bool URPPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (!GetOwningPlayerPawn()) return false;
    const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<URPWeaponComponent>();
    
    return WeaponComponent && WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}
