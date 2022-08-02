// Retrowave platformer game


#include "AI/RPBot.h"
#include "AI/RPAIController.h"
#include "Components/RPHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARPBot::ARPBot()
{
	PrimaryActorTick.bCanEverTick = false;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ARPAIController::StaticClass();

    HealthComponent = CreateDefaultSubobject<URPHealthComponent>("HealthComponent");
}

void ARPBot::BeginPlay()
{
	Super::BeginPlay();
    
    check(GetWorld());
    check(HealthComponent);
    check(GetMesh());

    HealthComponent->OnDeath.AddUObject(this, &ARPBot::OnDeath);
}

void ARPBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPBot::OnDeath() 
{
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.f);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

