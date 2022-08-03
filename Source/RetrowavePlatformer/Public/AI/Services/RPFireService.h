// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RPFireService.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API URPFireService : public UBTService
{
	GENERATED_BODY()

public:
    URPFireService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
