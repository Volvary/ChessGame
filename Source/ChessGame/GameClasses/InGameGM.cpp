// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameGM.h"

#include "GameClasses/GameBoard.h"

#include "GameClasses/Pieces/PieceFamily.h"

#include "Engine/World.h"

void AInGameGM::BeginPlay()
{
	GameBoard = (AGameBoard*)GetWorld()->SpawnActor<AGameBoard>(GameBoardSubclass);

	if (GameBoard != nullptr)
	{
		GameBoard->SetGameMode(this);
		GameBoard->PrepareGameBoard();
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
}

void AInGameGM::EndGame(EGameResult Result)
{

}

TArray<APieceFamily*> AInGameGM::GetPieceFamiliesToUse()
{
	TArray<APieceFamily*> Out;

	APieceFamily* Family = nullptr;

	Family = AvailableFamilies.Find(DefaultFamilyPlayer1)->GetDefaultObject();
	if (Family != nullptr)
	{
		Out.Add(Family);
		Players.Add(Family->FamilyType);
		ActivePlayer = Family->FamilyType;
	}

	Family = AvailableFamilies.Find(DefaultFamilyPlayer2)->GetDefaultObject();
	if (Family != nullptr)
	{
		Out.Add(Family);
		Players.Add(Family->FamilyType);
	}

	return Out;
}
