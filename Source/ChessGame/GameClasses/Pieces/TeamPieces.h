// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "PieceFamilyType.h"

#include "GameClasses/Moves.h"

#include "TeamPieces.generated.h"

class AChessPiece;

/**
 * This Object is a glorified Structure, due to limitation with Struct* in UPROPERTY(). 
 * (See GameBoard.h->GamePieces for context on the issue)
 */
UCLASS()
class CHESSGAME_API UTeamPieces : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EPieceFamilyType Family;

	UPROPERTY()
	bool bInCheck;

	UPROPERTY()
	TArray<AChessPiece*> TeamPieces;

	//Used to check for Checkmate and Stalemate
	UPROPERTY()
	TArray<FMove> ValidMoves;
	
};
