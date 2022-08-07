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
    virtual void Init() override;
    
    void SetGameDifficulty(FName Name);
    
    ERPGameDifficulty GetGameDifficulty() const { return GameDifficulty; };

    TArray<FName> GetGameDificultyList() const { return GameDifficultyNameList; };
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
    TMap<FName, ERPGameDifficulty> GameDificultyMap;

private:
    TArray<FName> GameDifficultyNameList;

    ERPGameDifficulty GameDifficulty {ERPGameDifficulty::Normal};
};
