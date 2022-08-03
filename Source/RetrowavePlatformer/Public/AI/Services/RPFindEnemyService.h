// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RPFindEnemyService.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API URPFindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
    URPFindEnemyService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
