#pragma once

#include "Moves.generated.h"

class AChessPiece;

USTRUCT()
struct FMove
{
	GENERATED_BODY()

	UPROPERTY()
	AChessPiece* Piece = nullptr;

	UPROPERTY()
	FIntVector StartTile = FIntVector(-1,-1,-1);

	UPROPERTY()
	FIntVector EndTile = FIntVector(-1,-1,-1);

	UPROPERTY()
	AChessPiece* CastlingPartner = nullptr;

	FMove(AChessPiece* TargetPiece, FIntVector Start, FIntVector End, AChessPiece* CastlingPiece = nullptr) : Piece(TargetPiece), StartTile(Start), EndTile(End), CastlingPartner(CastlingPiece){}
	
	FMove(){}

	FORCEINLINE bool operator == (const FMove Other) const
	{
		return (Piece == Other.Piece && StartTile == Other.StartTile && EndTile == Other.EndTile);
	}
};