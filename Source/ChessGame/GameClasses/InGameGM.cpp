// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameGM.h"

#include "GameClasses/GameBoard.h"

#include "GameClasses/Pieces/PieceFamily.h"

#include "Engine/World.h"

#include "GameClasses/InGamePlayerController.h"

#include "ChessGameInstance.h"

#include "ChessGame.h"

void AInGameGM::BeginPlay()
{
	GameBoard = (AGameBoard*)GetWorld()->SpawnActor<AGameBoard>(GameBoardSubclass);

	if (GameBoard != nullptr)
	{
		GameBoard->SetGameMode(this);
		GameBoard->PrepareGameBoard();

		bGameIsRunning = true;

		UpdateActivePlayerInterface();
	}
}

EPieceFamilyType AInGameGM::GetNextPlayer()
{
	EPieceFamilyType Out;

	int CurrentPlayer = Players.Find(ActivePlayer);

	Out = Players[ ((CurrentPlayer+1) % Players.Num()) ];

	return Out;
}

void AInGameGM::PlayerFinishedPlaying()
{

	bBlackIsPlaying = !bBlackIsPlaying;

	if (Players.Num() == 0)
	{
		return;
	}

	int Index = Players.Find(ActivePlayer);

	//If the end of the players was reached, loop over.
	Index = (Index + 1) % Players.Num();

	ActivePlayer = Players[Index];
	
	UpdateActivePlayerInterface();
}

void AInGameGM::EndGame(EGameResult Result)
{
	UE_LOG(LogChess, Warning, TEXT("Game Ended."));

	AInGamePlayerController* PC = Cast<AInGamePlayerController>(GetWorld()->GetFirstPlayerController());

	FText Winner;

	switch (Result)
	{
	case EGameResult::Cancelled:
		break;
	case EGameResult::Draw:
		break;
	case EGameResult::Stalemate:
		Winner = *PlayerNames.Find(ActivePlayer);
		break;
	case EGameResult::Checkmate:
		Winner = *PlayerNames.Find(ActivePlayer);
		break;
	case EGameResult::Forfeit:
		Winner = *PlayerNames.Find(GetNextPlayer());
		break;
	default:
		break;
	}

	PC->ShowEndGamePrompt(Result, Winner);
}

#pragma optimize("",off)
TArray<APieceFamily*> AInGameGM::GetPieceFamiliesToUse()
{
	TArray<APieceFamily*> Out;

	TSubclassOf<APieceFamily>* Family1Ref = nullptr;
	APieceFamily* Family1 = nullptr;

	TSubclassOf<APieceFamily>* Family2Ref = nullptr;
	APieceFamily* Family2 = nullptr;

	UChessGameInstance* GameInstance = Cast<UChessGameInstance>(GetWorld()->GetGameInstance());

	if (GameInstance != nullptr)
	{
		//Other player numbers not supported.
		if (GameInstance->PlayerList.Num() == 2)
		{
			Family1 = GameInstance->PlayerList[0].PlayerFamily;
			Family2 = GameInstance->PlayerList[1].PlayerFamily;
		}
		else
		{
			Family1Ref = GameInstance->AvailableFamilies.Find(DefaultFamilyPlayer1);
			if (Family1Ref != nullptr)
			{
				Family1 = Family1Ref->GetDefaultObject();
			}

			Family2Ref = GameInstance->AvailableFamilies.Find(DefaultFamilyPlayer2);
			if (Family2Ref != nullptr)
			{
				Family2 = Family2Ref->GetDefaultObject();
			}
		}
		
		if (Family1 != nullptr)
		{
			Out.Add(Family1);
			Players.Add(Family1->FamilyType);
			PlayersFamily.Add(Family1);
			ActivePlayer = Family1->FamilyType;

			if (Family1Ref == nullptr)
			{
				PlayerNames.Add(Family1->FamilyType, GameInstance->PlayerList[0].PlayerName);
			}
			else
			{
				PlayerNames.Add(Family1->FamilyType, FText::FromString("Player1") );
			}
		}

		if (Family2 != nullptr)
		{
			Out.Add(Family2);
			Players.Add(Family2->FamilyType);
			PlayersFamily.Add(Family2);
			
			if (Family2Ref == nullptr)
			{
				PlayerNames.Add(Family2->FamilyType, GameInstance->PlayerList[1].PlayerName);
			}
			else
			{
				PlayerNames.Add(Family2->FamilyType, FText::FromString("Player2"));
			}
		}
	}

	return Out;
}
#pragma optimize("",on)
///Auto-generated VA pragma optimize for debug.

APieceFamily* AInGameGM::GetSpecificFamily(EPieceFamilyType FamilyType)
{
	APieceFamily* Out = nullptr;

	for (APieceFamily* Family : PlayersFamily)
	{
		if (Family->FamilyType == FamilyType)
		{
			Out = Family;
		}
	}

	return Out;
}

void AInGameGM::PromotePiece(EPieceType Promotion)
{
	GameBoard->PromotePiece(Promotion);
}

void AInGameGM::UpdateActivePlayerInterface()
{
	AInGamePlayerController* PC = Cast<AInGamePlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC != nullptr)
	{
		FText* ActivePlayerName = PlayerNames.Find(ActivePlayer);

		if (ActivePlayerName != nullptr)
		{
			PC->UpdateActivePlayer(*ActivePlayerName);
		}

	}
}
