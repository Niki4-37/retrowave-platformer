// Copyright Epic Games, Inc. All Rights Reserved.


#include "RetrowavePlatformerGameModeBase.h"
#include "Public/Player/RPBaseCharacter.h"
#include "Public/Player/RPPlayerController.h"
#include "Public/Environment/RPTile.h"
#include "Public/Player/RPPlayerController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

#include "Public/AI/RPAIController.h"
#include "Public/AI/RPBot.h"

DEFINE_LOG_CATEGORY_STATIC(RPGameMode_LOG, All, All);

ARetrowavePlatformerGameModeBase::ARetrowavePlatformerGameModeBase() 
{
    DefaultPawnClass = ARPBaseCharacter::StaticClass();
    PlayerControllerClass = ARPPlayerController::StaticClass();
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
                ArrayBool[i][j] = FMath::RandHelper(1);
            }

            TileCoords.Add(FTileCoords(i, j, ArrayBool[i][j]));
        }
    }

    bEmptySpawnTransform = true;

    if (!GetWorld()) return;

    for (const auto& TileCoord : TileCoords)
    {
        if (!TileCoord.Enabled) continue;

        FTransform SpawnTransform(
            FTransform(FRotator::ZeroRotator, FVector(2050.f * TileCoord.MultiplierX, 2050.f * TileCoord.MultiplierY, 0.f)));

        FActorSpawnParameters Param;
        Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        const auto NewTile = GetWorld()->SpawnActor<ARPTile>(TileClass, SpawnTransform, Param);
        //===== Set player Transform for spawning
        if (!bEmptySpawnTransform) continue;
        PlayerSpawnTransform = SpawnTransform;
        UE_LOG(RPGameMode_LOG, Display, TEXT("PlayerSpawnTransform Locacion: %s"), *PlayerSpawnTransform.GetLocation().ToString());
        bEmptySpawnTransform = false;
    }
    //=======
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
    }
    //=======
}

void ARetrowavePlatformerGameModeBase::RestartPlayer(AController* NewPlayer) 
{
    // to delete
    /*FVector SpawnOrigin = PlayerSpawnTransform.GetLocation();
    FRotator StartRotation = FRotator::ZeroRotator;
   
    if (SpawnOrigin == FVector::ZeroVector)
    {
        Super::RestartPlayer(NewPlayer);
        return;
    }

    FString TestText =
        NewPlayer->GetPawn() ? NewPlayer->GetPawn()->GetName() : "No player pawn";
    UE_LOG(RPGameMode_LOG, Display, TEXT("Has Actor: %s"), *TestText);
    
    if (!NewPlayer->GetPawn() && GetDefaultPawnClassForController(NewPlayer))
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        UE_LOG(RPGameMode_LOG, Display, TEXT("SpawnOrigin: %s"), *SpawnOrigin.ToString());
        
        APawn* ResultPawn =
            GetWorld()->SpawnActor<APawn>(GetDefaultPawnClassForController(NewPlayer), SpawnOrigin + FVector(0.f, 0.f, 200.f), StartRotation, SpawnInfo);
        if (!ResultPawn)
        {
            UE_LOG(RPGameMode_LOG, Warning, TEXT("Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(DefaultPawnClass), &SpawnOrigin);
        }
        UE_LOG(RPGameMode_LOG, Display, TEXT("Actor spawned: %s at %s"), *ResultPawn->GetName(), *ResultPawn->GetActorLocation().ToString());
        NewPlayer->SetPawn(ResultPawn);
    }

    if (!NewPlayer->GetPawn())
    {
        NewPlayer->FailedToSpawnPawn();
    }
    else
    {
        NewPlayer->Possess(NewPlayer->GetPawn());

        if (!NewPlayer->GetPawn())
        {
            NewPlayer->FailedToSpawnPawn();
        }
        else
        {
            NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

            FRotator NewControllerRot = StartRotation;
            NewControllerRot.Roll = 0.f;
            NewPlayer->SetControlRotation(NewControllerRot);

            SetPlayerDefaults(NewPlayer->GetPawn());
        }
    }*/
}


void ARetrowavePlatformerGameModeBase::ResetOnePlayer(AController* Controller) 
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
}