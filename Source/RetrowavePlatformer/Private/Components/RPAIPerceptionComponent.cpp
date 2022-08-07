// Retrowave platformer game

#include "Components/RPAIPerceptionComponent.h"
#include "Components/RPHealthComponent.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "Player/RPPlayerController.h"

AActor* URPAIPerceptionComponent::GetClosesEnemy() const
{
    TArray<AActor*> PercievedActorsList;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercievedActorsList);
    if (PercievedActorsList.Num() == 0) return nullptr;

    AActor* RequiredPawn = nullptr;

    for (const auto& FoundActor : PercievedActorsList)
    {
        const auto FoundPawn = Cast<APawn>(FoundActor);
        if (!FoundPawn) continue;
        
        const auto HealthComponent = FoundPawn->FindComponentByClass<URPHealthComponent>();

        const auto FoundPawnController = Cast<ARPPlayerController>(FoundPawn->GetController());
        if (FoundPawnController)
        {
            if (!HealthComponent || HealthComponent->IsDead()) continue;
            RequiredPawn = FoundActor;
        }
    }

    return RequiredPawn;
}