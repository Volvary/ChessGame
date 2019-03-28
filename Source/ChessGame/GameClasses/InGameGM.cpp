// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameGM.h"

#include "GameClasses/GameBoard.h"
#include "Engine/World.h"

void AInGameGM::BeginPlay()
{
	GameBoard = (AGameBoard*)GetWorld()->SpawnActor<AGameBoard>(GameBoardSubclass);

	if (GameBoard != nullptr)
	{
		GameBoard->SetGameMode(this);
	}
}

void AInGameGM::PlayerFinishedPlaying()
{
	bBlackIsPlaying = !bBlackIsPlaying;
}
