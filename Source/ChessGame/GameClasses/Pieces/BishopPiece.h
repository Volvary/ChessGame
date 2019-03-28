// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameClasses/Pieces/ChessPiece.h"
#include "BishopPiece.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API ABishopPiece : public AChessPiece
{
	GENERATED_BODY()
	

public:

	// Called when the game starts or when spawned
	void BeginPlay() override;

};
