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
#include "RPGameInstance.h"

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
    
    SetGameDifficulty();
    
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
    
    PutPlayerToEnabledTile();

    SetCurrentGameState(ERPGameState::InPlay);
}

void ARetrowavePlatformerGameModeBase::PutPlayerToEnabledTile()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto PlayerController = Cast<ARPPlayerController>(It->Get());
        if (!PlayerController) continue;

        PlayerSpawnTransform =FTransform(
            PlayerSpawnTransform.GetRotation(), 
            PlayerSpawnTransform.GetLocation() + FVector(0.f, 0.f, 200.f));
        
        RestartPlayerAtTransform(PlayerController, PlayerSpawnTransform);

        const auto Player = Cast<ARPBaseCharacter>(PlayerController->GetPawn());
        if (!Player) continue;
        const auto HealthComponent = Player->FindComponentByClass<URPHealthComponent>();
        if (!HealthComponent) continue;
        HealthComponent->OnDeath.AddUObject(this, &ARetrowavePlatformerGameModeBase::OnDeath);
    }
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

void ARetrowavePlatformerGameModeBase::SpawnTile(const FTileCoords& Coords, FLevelConfiguration Configuration)
{
    if (!Coords.Enabled) return;

    FVector TileLocation(FVector(TileOffset * Coords.MultiplierX, TileOffset * Coords.MultiplierY, 0.f));
    FTransform SpawnTransform(FTransform(FRotator::ZeroRotator, TileLocation));

    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    const auto NewTile = GetWorld()->SpawnActorDeferred<ARPTile>(
        TileClass, 
        SpawnTransform, 
        this, 
        nullptr, 
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
    
    if (NewTile)
    {
        NewTile->SetTileConfiguration(Configuration);
        if (bFirstEnabledTile)
        {
            NewTile->SetTileConfiguration(FLevelConfiguration());
        }
        NewTile->FinishSpawning(SpawnTransform);
    }

    // Get first enable tile for spawning
    if (!bFirstEnabledTile) return;
    PlayerSpawnTransform = SpawnTransform;
    bFirstEnabledTile = false;
}

void ARetrowavePlatformerGameModeBase::FillTheLevel()
{
    bFirstEnabledTile = true;

    if (!GetWorld()) return;

    uint8 TileIndex = 0;
    uint8 BotsModifier = 0;

    for (const auto& TileCoord : TileCoords)
    {
        if (TileIndex > 15 && TileIndex <= 25)
        {
            BotsModifier = 1;
        }
        if (TileIndex > 25)
        {
            BotsModifier = 2;
        }

        FLevelConfiguration LevelConfiguration(FLevelConfiguration(
            FMath::RandHelper(2 + BotsModifier + DifficultyModifier), 
            FMath::RandHelper(2 + DifficultyModifier)));

        SpawnTile(TileCoord, LevelConfiguration);
        
        if (!TileCoord.Enabled) continue;
        ++TileIndex;
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

void ARetrowavePlatformerGameModeBase::SetGameDifficulty() 
{
    if (!GetWorld()) return;
    
    auto GameInstance = GetWorld()->GetGameInstance<URPGameInstance>();
    if (!GameInstance) return;
    const auto Difficulty = GameInstance->GetGameDifficulty();

    UE_LOG(RPGameMode_LOG, Display, TEXT("%s"), *UEnum::GetValueAsString(Difficulty));

    switch (Difficulty)
    {
        case ERPGameDifficulty::Easy: 
            DifficultyModifier = 0;
            break;
        case ERPGameDifficulty::Normal: 
            DifficultyModifier = 1;
            break;
        case ERPGameDifficulty::Hard: 
            DifficultyModifier = 2; 
            break;
        default: break;
    }
}
