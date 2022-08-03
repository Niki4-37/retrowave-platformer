// Retrowave platformer game


#include "AI/Services/RPFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/RPTurret.h"
#include "Components/RPWeaponComponent.h"

URPFireService::URPFireService() 
{
    NodeName = "Fire";
}

void URPFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    const auto BotController = OwnerComp.GetAIOwner();
    const auto Turret = Cast<ARPTurret>(BotController->GetPawn());
    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Turret)
    {
        Turret->SetWeaponStartFire(HasAim);
    }
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
