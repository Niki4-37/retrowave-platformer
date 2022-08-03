// Retrowave platformer game


#include "AI/RPBot.h"
#include "AI/RPAIController.h"
#include "Components/RPHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"

// Sets default values
ARPBot::ARPBot()
{
	PrimaryActorTick.bCanEverTick = false;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ARPAIController::StaticClass();

    HealthComponent = CreateDefaultSubobject<URPHealthComponent>("HealthComponent");

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
    } 
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
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
    }

    const auto BotController = Cast<AAIController>(Controller);
    if (BotController && BotController->BrainComponent)
    {
        BotController->BrainComponent->Cleanup();
    }
 
    SetLifeSpan(5.f);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

