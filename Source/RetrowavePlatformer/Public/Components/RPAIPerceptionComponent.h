// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "RPAIPerceptionComponent.generated.h"

class ARPPlayerController;

UCLASS()
class RETROWAVEPLATFORMER_API URPAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
    AActor* GetClosesEnemy() const;
};
