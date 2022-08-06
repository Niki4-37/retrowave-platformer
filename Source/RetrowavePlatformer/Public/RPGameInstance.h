// Retrowave platformer game

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPTypes.h"
#include "RPGameInstance.generated.h"

UCLASS()
class RETROWAVEPLATFORMER_API URPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    void SetGameDifficult(ERPGameDifficult Difficult){GameDifficult = Difficult; };
    ERPGameDifficult GetGameDifficult() const { return GameDifficult; };
    
private:
    ERPGameDifficult GameDifficult {ERPGameDifficult::Normal};
	
};
