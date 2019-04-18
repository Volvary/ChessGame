#pragma once

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EPieceMovement : uint8
{
	King = 1,
	Rook = 2,
	Bishop = 4,
	Knight = 8,
	Pawn = 16, 
	Castling = 32,	//Marks the piece as plausible for Castling.
	MAX_VALUE = 64
};

ENUM_CLASS_FLAGS(EPieceMovement);

///Used solely for Interface To Code translation.
UENUM(BlueprintType)
enum class EPieceType : uint8
{
	None,
	King,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn
};