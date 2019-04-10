#pragma once

UENUM(BlueprintType)
enum class SoundEvents : uint8
{
	GenericPieceSelection,
	GenericPieceMove,
	PawnMove,
	KnightMove,
	RookMove,
	BishopMove,
	QueenMove,
	KingMove,
	Check,
	GenericGameEnd,
	Checkmate,
	Stalemate,
	Draw,
};