// Retrowave platformer game


#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::SetHealth(float NewHealth) 
{
    Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    check(GetWorld());
	SetHealth(MaxHealth);

    const auto ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeAnyDamage);
    }
}

void UHealthComponent::TakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.f || IsDead()) return;

    SetHealth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
    } 
}

