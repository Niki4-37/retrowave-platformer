// Copyright Epic Games, Inc. All Rights Reserved.


#include "RetrowavePlatformerGameModeBase.h"
#include "Public/Player/RPBaseCharacter.h"
#include "Public/Player/RPPlayerController.h"
#include "Public/Environment/RPTile.h"
#include "Public/Player/RPPlayerController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "UI/RPHUD.h"
#include "EngineUtils.h"
#include "Components/RPHealthComponent.h"

#include "Public/AI/RPAIController.h"
#include "Public/AI/RPBot.h"

DEFINE_LOG_CATEGORY_STATIC(RPGameMode_LOG, All, All);

ARetrowavePlatformerGameModeBase::ARetrowavePlatformerGameModeBase() 
{
    DefaultPawnClass = ARPBaseCharacter::StaticClass();
    PlayerControllerClass = ARPPlayerController::StaticClass();
    HUDClass = ARPHUD::StaticClass();
}

void ARetrowavePlatformerGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) 
{
    Super::InitGame(MapName, Options, ErrorMessage);

    //======= Generate Map 6X10 Tiles
    int ArrayBool[6][10]{0};

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            ArrayBool[i][j] = FMath::RandBool();

            if (i == 0 && j == 0)
            {
                ArrayBool[0][0] = 0;
            }

            if (ArrayBool[0][j] && i > 0 && j > 0)
            {
                ArrayBool[i][j] = 1;
            }

            if (ArrayBool[i][0] && i > 0 && j > 0)
            {
                ArrayBool[i][j] = 1;
            }

            if (i > 0 && j > 0 && ArrayBool[i - 1][j] && ArrayBool[i][j - 1] && ArrayBool[i - 1][j - 1])
            {
                ArrayBool[i][j] = FMath::RandBool();
            }

            TileCoords.Add(FTileCoords(i, j, ArrayBool[i][j]));
        }
    }

    FillTheLevel();
}

void ARetrowavePlatformerGameModeBase::StartPlay() 
{
    Super::StartPlay();
    
    //======= Set player in Enable tile ======
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto PlayerController = Cast<ARPPlayerController>(It->Get());
        if (!PlayerController) continue;
        UE_LOG(RPGameMode_LOG, Display, TEXT("Cast ARPPlayerController"));

        //to delete
        //RestartPlayer(PlayerController);
        
        PlayerSpawnTransform = FTransform(PlayerSpawnTransform.GetRotation(), PlayerSpawnTransform.GetLocation() + FVector(0.f, 0.f, 200.f));
        RestartPlayerAtTransform(PlayerController, PlayerSpawnTransform);

        const auto Player = Cast<ARPBaseCharacter>(PlayerController->GetPawn());
        if (!Player) continue;
        const auto HealthComponent = Player->FindComponentByClass<URPHealthComponent>();
        if (!HealthComponent) continue;
        HealthComponent->OnDeath.AddUObject(this, &ARetrowavePlatformerGameModeBase::OnDeath);
    }
    //=======

    SetCurrentGameState(ERPGameState::InPlay);
}

bool ARetrowavePlatformerGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto bIsPauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (bIsPauseSet)
    {
        SetCurrentGameState(ERPGameState::Paused);
    }
    return bIsPauseSet;
}

bool ARetrowavePlatformerGameModeBase::ClearPause()
{
    const auto bIsPauseCleared = Super::ClearPause();
    if (bIsPauseCleared)
    {
        SetCurrentGameState(ERPGameState::InPlay);
    }
    return bIsPauseCleared;
}

void ARetrowavePlatformerGameModeBase::RestartPlayer(AController* NewPlayer) {}

void ARetrowavePlatformerGameModeBase::FillTheLevel()
{
    bEmptySpawnTransform = true;

    if (!GetWorld()) return;

    for (const auto& TileCoord : TileCoords)
    {
        if (!TileCoord.Enabled) continue;

        FVector TileLocation(FVector(TileOffset * TileCoord.MultiplierX, TileOffset * TileCoord.MultiplierY, 0.f));
        FTransform SpawnTransform(FTransform(FRotator::ZeroRotator, TileLocation));

        FActorSpawnParameters Param;
        Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        //const auto NewTile = GetWorld()->SpawnActor<ARPTile>(TileClass, SpawnTransform, Param);
        const auto NewTile = GetWorld()->SpawnActorDeferred<ARPTile>(TileClass, SpawnTransform, this ,nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);  
        if (NewTile)
        {
            NewTile->SetTileConfiguration(FLevelConfiguration(FMath::RandBool(), FMath::RandBool()));
            NewTile->FinishSpawning(SpawnTransform);
        } 

        // Get first enable tile for spawning
        if (!bEmptySpawnTransform) continue;
        PlayerSpawnTransform = SpawnTransform;
        UE_LOG(RPGameMode_LOG, Display, TEXT("PlayerSpawnTransform Locacion: %s"), *PlayerSpawnTransform.GetLocation().ToString());
        bEmptySpawnTransform = false;
    }
}

void ARetrowavePlatformerGameModeBase::ResetOnePlayer(AController* Controller) 
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
}

void ARetrowavePlatformerGameModeBase::OnDeath() 
{
    GameOver();
}

void ARetrowavePlatformerGameModeBase::GameOver() 
{
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetCurrentGameState(ERPGameState::GameOver);
}

void ARetrowavePlatformerGameModeBase::SetCurrentGameState(ERPGameState State) 
{
    if (CurrentRPGameState == State) return;

    CurrentRPGameState = State;
    OnGameStateChanged.Broadcast(State);
}
