// Retrowave platformer game


#include "Components/RPHealthComponent.h"

URPHealthComponent::URPHealthComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void URPHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

void URPHealthComponent::TryToHeal() 
{
    SetHealth(Health + 20.f);
}

void URPHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    check(GetWorld());
    check(MaxHealth > 0.f); 
    SetHealth(MaxHealth);

    const auto ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &URPHealthComponent::TakeAnyDamage);
    }
}

void URPHealthComponent::TakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.f || IsDead()) return;

    SetHealth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
}
