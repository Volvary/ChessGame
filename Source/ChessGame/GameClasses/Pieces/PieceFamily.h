// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PieceFamilyType.h"
#include "GameFramework/Actor.h"

#include "PieceFamily.generated.h"

class AKingPiece;
class AQueenPiece;
class ABishopPiece;
class AKnightPiece;
class ARookPiece;
class APawnPiece;

/**
 * 
 */
UCLASS()
class CHESSGAME_API APieceFamily : public AActor
{

	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Piece Family")
	EPieceFamilyType FamilyType;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Piece Family | Pieces")
	TSubclassOf<AKingPiece> KingPiece;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Piece Family | Pieces")
	TSubclassOf<AQueenPiece> QueenPiece;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Piece Family | Pieces")
	TSubclassOf<ABishopPiece> BishopPiece;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Piece Family | Pieces")
	TSubclassOf<AKnightPiece> KnightPiece;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Piece Family | Pieces")
	TSubclassOf<ARookPiece> RookPiece;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Piece Family | Pieces")
	TSubclassOf<APawnPiece> PawnPiece;

	APieceFamily();
	~APieceFamily();
};
