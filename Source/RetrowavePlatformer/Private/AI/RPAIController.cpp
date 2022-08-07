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
        GenetateRotationYaw();
    }
}

void ARPAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto AimActor = FocusOnActor();

    SetFocus(AimActor);
    
    const auto StaticEnemy = Cast<ARPTurret>(GetPawn());
    if (StaticEnemy)
    {
        if (AimActor)
        {
            const auto EnemyLocation = AimActor->GetActorLocation();
            const auto SelfLocation = StaticEnemy->GetActorLocation();

            const auto RiquiredRotation = UKismetMathLibrary::FindLookAtRotation(SelfLocation, EnemyLocation);
            StaticEnemy->SetActorRotation(RiquiredRotation);
        }
        else
        {
            StaticEnemy->AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));
        }
    }
}

AActor* ARPAIController::FocusOnActor()
{
    if (!GetBlackboardComponent()) return nullptr;

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

void ARPAIController::GenetateRotationYaw() 
{
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
    RotationYaw = FMath::RandRange(0.1f, 0.5f) * Direction;
}
