// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessGameGameModeBase.h"

#include "Generic/GameResult.h"

#include "GameClasses/Pieces/PieceFamilyType.h"

#include "InGameGM.generated.h"

class AGameBoard;
class APieceFamily;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AInGameGM : public AChessGameGameModeBase
{
	GENERATED_BODY()
	
protected:
	AGameBoard* GameBoard;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pieces")
	TMap<EPieceFamilyType, TSubclassOf<APieceFamily>> AvailableFamilies;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pieces")
	EPieceFamilyType DefaultFamilyPlayer1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pieces")
	EPieceFamilyType DefaultFamilyPlayer2;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AGameBoard> GameBoardSubclass;

	EPieceFamilyType ActivePlayer;

	TArray<EPieceFamilyType> Players;

	bool bBlackIsPlaying = false;

public:
	void BeginPlay();

	EPieceFamilyType GetActivePlayer(){ return ActivePlayer; }

	EPieceFamilyType GetNextPlayer();

	bool IsBlackPlaying() { return bBlackIsPlaying; }

	void PlayerFinishedPlaying();

	void EndGame(EGameResult Result);

	TArray<APieceFamily*> GetPieceFamiliesToUse();
};