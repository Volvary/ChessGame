// Fill out your copyright notice in the Description page of Project Settings.

#include "InGamePlayerController.h"
#include "ChessHUD.h"

#include "GameClasses/InGameGM.h"

#include "ChessGame/GameClasses/Interfaces/TurnStatusInterface.h"
#include "ChessGame/GameClasses/Interfaces/EndGamePrompt.h"

void AInGamePlayerController::BeginPlay()
{
	AChessHUD* HUD = Cast<AChessHUD>(GetHUD());

	if (HUD != nullptr)
	{
		TurnStatus = HUD->ShowTurnStatusInterface();
		HUD->ShowGameStatusInterface();
	}
}

void AInGamePlayerController::UpdateActivePlayer(const FText& ActivePlayer)
{
	if (TurnStatus == nullptr)
	{
		AChessHUD* HUD = Cast<AChessHUD>(GetHUD());

		if (HUD != nullptr)
		{
			TurnStatus = HUD->ShowTurnStatusInterface();
		}
	}

	if (TurnStatus != nullptr)
	{
		TurnStatus->UpdateActivePlayer(ActivePlayer);
	}
}

void AInGamePlayerController::ShowEndGamePrompt(EGameResult Result, FText Winner)
{
	AChessHUD* HUD = Cast<AChessHUD>(GetHUD());

	UEndGamePrompt* EndGame = HUD->ShowEndGamePrompt();

	

	if (EndGame != nullptr)
	{
		EndGame->SetGameResult(Result, Winner);
	}
}