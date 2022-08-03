// Retrowave platformer game

#include "AI/Services/RPFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/RPAIPerceptionComponent.h"

URPFindEnemyService::URPFindEnemyService() 
{
    NodeName = "Find Enemy";
}

void URPFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto BotController = OwnerComp.GetAIOwner();
        const auto BotPerceptionComponent = BotController->FindComponentByClass<URPAIPerceptionComponent>();
        if (BotPerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, BotPerceptionComponent->GetClosesEnemy());
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

