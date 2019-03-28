// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Generic/BoardLayers.h"

#include "Generic/BoardActionInformations.h"

#include "GameBoard.generated.h"

class ABoardTile;

class AChessPiece;
class APawnPiece;
class ARookPiece;
class AKnightPiece;
class ABishopPiece;
class AKingPiece;
class AQueenPiece;

class AInGameGM;

UCLASS()
class CHESSGAME_API AGameBoard : public AActor
{
	GENERATED_BODY()

protected:

	//******* Params ********/
	//The number of dimensions the board plays in.
	int GameBoardDimensions;

	//The length of each dimensions.
	int GameBoardSize;

	//Used to define the space between the spawned pieces.
	float TileSize;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chess Board | Tiles")
	TSubclassOf<ABoardTile> BoardTileSubclass = nullptr;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chess Board | Pieces")
	TSubclassOf<APawnPiece> PawnSubclass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chess Board | Pieces")
	TSubclassOf<ARookPiece> RookSubclass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chess Board | Pieces")
	TSubclassOf<AKnightPiece> KnightSubclass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chess Board | Pieces")
	TSubclassOf<ABishopPiece> BishopSubclass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chess Board | Pieces")
	TSubclassOf<AQueenPiece> QueenSubclass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chess Board | Pieces")
	TSubclassOf<AKingPiece> KingSubclass = nullptr;


	//******** Runtime Variables ********/

	UPROPERTY(Transient)
	TArray<ABoardTile*> GameBoard;

	UPROPERTY(Transient)
	TArray<AChessPiece*> GamePieces;

	UPROPERTY(Transient)
	AInGameGM* GameMode;

	UPROPERTY(Transient)
	ABoardTile* SelectedTile;

	UPROPERTY(Transient)
	TMap<ABoardTile*, EBoardTileState> MarkedTiles;

public:	
	// Sets default values for this actor's properties
	AGameBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SelectTile(ABoardTile* NewSelected);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGameMode(AInGameGM* GM);

	void PrepareGameBoard();

	ABoardTile* GetTileAt(int X, int Y, int Z = 0);
	ABoardTile* GetTileAt(FIntVector Position);

	//UFUNCTION since it is bound to Delegates.
	UFUNCTION()
	void ClickedOnTile(ABoardTile* ClickedTile);

	void FindValidMoves();

	bool CanBeMovedOn(ABoardTile* TileToMoveOn, AChessPiece* MovingPiece);

	void Move(ABoardTile* StartTile, ABoardTile* EndTile);

	TMap<FIntVector, EBoardTileState> TestPawnMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestRookMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestBishopMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestKingMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestKnightMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestCastlingMovement(FIntVector Position, AChessPiece* Piece);
};
