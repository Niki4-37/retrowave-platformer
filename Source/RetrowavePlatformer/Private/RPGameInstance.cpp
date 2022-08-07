// Retrowave platformer game


#include "RPGameInstance.h"

void URPGameInstance::Init() 
{
    Super::Init();

    for (const auto& GameDificultyPair : GameDificultyMap)
    {
        GameDifficultyNameList.Add(GameDificultyPair.Key);
    }
}

void URPGameInstance::SetGameDifficulty(FName Name)
{
    if (GameDificultyMap.Contains(Name))
    {
        GameDifficulty = GameDificultyMap[Name];
    }
}
