// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessGameInstance.h"

#include "GameClasses/Pieces/PieceFamily.h"

void UChessGameInstance::AddToPlayerList(FText PlayerName, TSubclassOf<APieceFamily> FamilyType)
{
	if (FamilyType != nullptr)
	{
		PlayerList.Add(FPlayerInfo(PlayerName, Cast<APieceFamily>(FamilyType->GetDefaultObject() ) ) );
	}
}
