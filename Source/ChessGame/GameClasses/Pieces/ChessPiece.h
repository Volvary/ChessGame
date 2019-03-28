// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameClasses/Pieces/PieceMovement.h"

#include "ChessPiece.generated.h"

UCLASS()
class CHESSGAME_API AChessPiece : public AActor
{
	GENERATED_BODY()

protected:

	//Used as a bitwise. (Queen = Rook | Bishop)
	UPROPERTY(meta = ( Bitmask, BitmaskEnum = "EPieceMovement"))
	EPieceMovement Movement = (EPieceMovement) 0;

	//Only set on Pieces with the Pawn movement, serves to identify when a pawn can be attacked by the "En Passant" special move.
	//Set when a pawn moves forward twice and unset when moved anymore.
	bool bCanBeEnPassant = false;

	//Mostly used for Pawn moving 2 squares and for Castling.
	bool bHasMoved = false;

	//Using bool since only two teams exist. Move to enum if adding more teams (1v1v1 chess)
	UPROPERTY(BlueprintReadOnly)
	bool bIsBlackTeam = false;

public:
	
public:	
	// Sets default values for this actor's properties
	AChessPiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsBlackTeam() { return bIsBlackTeam; }

	void SetTeam(bool bIsBlackPiece) { bIsBlackTeam = bIsBlackPiece; ChangeColor(); }

	EPieceMovement GetAvailableMovement() {return Movement;}

	bool HasMoved() {return bHasMoved;}

	void SetHasMoved(bool NewMoved) {bHasMoved = NewMoved;}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeColor();

};
