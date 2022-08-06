// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPDefaultPickup.generated.h"

class USphereComponent;

UCLASS()
class RETROWAVEPLATFORMER_API ARPDefaultPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPDefaultPickup();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionComponent;

	virtual void BeginPlay() override;
    
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    virtual bool GivePickupToPlayer(APawn* Pawn) { return false; };
};
