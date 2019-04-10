// Fill out your copyright notice in the Description page of Project Settings.

#include "KingPiece.h"

void AKingPiece::BeginPlay()
{
	Movement = EPieceMovement::King | EPieceMovement::Castling;
	bIsRoyal = true;
}
