// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessGameGameModeBase.h"
#include "InGameGM.generated.h"

class AGameBoard;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AInGameGM : public AChessGameGameModeBase
{
	GENERATED_BODY()
	
protected:
	AGameBoard* GameBoard;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AGameBoard> GameBoardSubclass;

	bool bBlackIsPlaying = false;

public:
	void BeginPlay();

	bool IsBlackPlaying() { return bBlackIsPlaying; }

	void PlayerFinishedPlaying();
};
