// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPTypes.h"
#include "RetrowavePlatformerGameModeBase.generated.h"

class ARPTile;

USTRUCT()
struct FTileCoords
{
    GENERATED_USTRUCT_BODY()

    FTileCoords() {};
    FTileCoords(int X, int Y, int bE) : MultiplierX(X), MultiplierY(Y), Enabled(bE) {}

    int MultiplierX {0};
    int MultiplierY {0};
    int Enabled {0};
};

UCLASS()
class RETROWAVEPLATFORMER_API ARetrowavePlatformerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ARetrowavePlatformerGameModeBase();

    FOnGameStateChangedSignature OnGameStateChanged;

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    virtual void StartPlay() override;

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Environment")
    TSubclassOf<ARPTile> TileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Environment")
    float TileOffset {2100.f};

    TArray <FTileCoords> TileCoords;

    virtual void RestartPlayer(class AController* NewPlayer) override;

private:
    bool bEmptySpawnTransform{true};
    FTransform PlayerSpawnTransform;

    ERPGameState CurrentRPGameState = ERPGameState::WaitingToStart;

    void FillTheLevel();

    void ResetOnePlayer(AController* Controller);

    void OnDeath();

    void GameOver();

    void SetCurrentGameState(ERPGameState State);
};


