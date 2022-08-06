// Retrowave platformer game


#include "Pickups/RPDefaultPickup.h"
#include "Components/SphereComponent.h"
#include "Player/RPBaseCharacter.h"

#include "DrawDebugHelpers.h"

ARPDefaultPickup::ARPDefaultPickup()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    SetRootComponent(CollisionComponent);
    CollisionComponent->SetSphereRadius(50.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ARPDefaultPickup::BeginPlay()
{
	Super::BeginPlay();
    check(CollisionComponent);
    check(GetWorld());	
}

void ARPDefaultPickup::NotifyActorBeginOverlap(AActor* OtherActor) 
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Player = Cast<ARPBaseCharacter>(OtherActor);
    if (!Player) return;
    if (GivePickupToPlayer(Player))
    {
        Destroy();
    }
}

