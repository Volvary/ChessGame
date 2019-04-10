// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameClasses/Pieces/ChessPiece.h"
#include "KnightPiece.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AKnightPiece : public AChessPiece
{
	GENERATED_BODY()

public:

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
};
