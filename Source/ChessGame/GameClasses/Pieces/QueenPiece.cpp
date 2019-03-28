// Fill out your copyright notice in the Description page of Project Settings.

#include "QueenPiece.h"

void AQueenPiece::BeginPlay()
{
	Movement = EPieceMovement::Rook | EPieceMovement::Bishop;
}
