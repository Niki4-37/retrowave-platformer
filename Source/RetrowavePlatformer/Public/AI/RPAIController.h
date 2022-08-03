// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPAIController.generated.h"

class URPAIPerceptionComponent;

UCLASS()
class RETROWAVEPLATFORMER_API ARPAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    ARPAIController();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Behavior")
    URPAIPerceptionComponent* BotPerceptionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Behavior")
    FName FocusOnKeyName = "EnemyActor";

    virtual void OnPossess(APawn* InPawn) override;

    virtual void Tick(float DeltaTime) override;

private:
    AActor* FocusOnActor();
};
