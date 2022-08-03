// Retrowave platformer game

#include "Components/RPAIPerceptionComponent.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "Player/RPPlayerController.h"

AActor* URPAIPerceptionComponent::GetClosesEnemy() const
{
    TArray<AActor*> PercievedActorsList;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercievedActorsList);
    if (PercievedActorsList.Num() == 0) return nullptr;

    const auto BotController = Cast<AAIController>(GetOwner());
    if (!BotController) return nullptr;

    const auto Bot = BotController->GetPawn();
    if (!Bot) return nullptr;

    AActor* RequiredPawn = nullptr;

    for (const auto& FoundActor : PercievedActorsList)
    {
        const auto FoundPawn = Cast<APawn>(FoundActor);
        if (!FoundPawn) continue;
        
        const auto FoundPawnController = Cast<ARPPlayerController>(FoundPawn->GetController());
        if (FoundPawnController)
        {
            RequiredPawn = FoundActor;
        }
    }

    return RequiredPawn;
}