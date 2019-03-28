// Fill out your copyright notice in the Description page of Project Settings.

#include "RookPiece.h"

void ARookPiece::BeginPlay()
{
	Movement = EPieceMovement::Rook | EPieceMovement::Castling;

}
