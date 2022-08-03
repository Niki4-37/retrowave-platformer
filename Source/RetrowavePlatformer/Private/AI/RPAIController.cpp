// Retrowave platformer game

#include "AI/RPAIController.h"
#include "AI/RPBot.h"
#include "AI/RPTurret.h"
#include "Components/RPAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetMathLibrary.h"

ARPAIController::ARPAIController()
{
    BotPerceptionComponent = CreateDefaultSubobject<URPAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*BotPerceptionComponent);
}

void ARPAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto MovableEnemy = Cast<ARPBot>(InPawn);
    if (MovableEnemy)
    {
        RunBehaviorTree(MovableEnemy->BehaviorTreeAsset);
    }

    const auto StaticEnemy = Cast<ARPTurret>(InPawn);
    if (StaticEnemy)
    {
        RunBehaviorTree(StaticEnemy->BehaviorTreeAsset);
    }
}

void ARPAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto AimActor = FocusOnActor();
    //const auto AimActor = BotPerceptionComponent->GetClosesEnemy();
    SetFocus(AimActor);
    
    //??? Turret class function ======================
    const auto StaticEnemy = Cast<ARPTurret>(GetPawn());
    if (StaticEnemy)
    {
        const auto EnemyLocation = AimActor ? AimActor->GetActorLocation() : FVector::ZeroVector;
        const auto SelfLocation = StaticEnemy->GetActorLocation();
        
        const auto RiquiredRotation =
            UKismetMathLibrary::FindLookAtRotation(SelfLocation, EnemyLocation);
        StaticEnemy->SetActorRotation(RiquiredRotation);
    }
    //====================
}

AActor* ARPAIController::FocusOnActor()
{
    if (!GetBlackboardComponent()) return nullptr;

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
