// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Generic/BoardLayers.h"

#include "Generic/BoardActionInformations.h"

#include "GameClasses/Pieces/TeamPieces.h"

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


	//******** Runtime Variables ********/

	UPROPERTY()
	TArray<ABoardTile*> GameBoard;

	UPROPERTY()
	TArray<UTeamPieces*> GamePieces;

	UPROPERTY(Transient)
	AInGameGM* GameMode;

	UPROPERTY(Transient)
	ABoardTile* SelectedTile;

	UPROPERTY(Transient)
	TMap<ABoardTile*, EBoardTileState> MarkedTiles;

	UPROPERTY(Transient)
	AChessPiece* MostRecentMove;

	/// Invalidated after every pseudo-move logic.
	FMove* PotentialMovement = nullptr;

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

	TMap<FIntVector, EBoardTileState> FindValidMovesForPiece(AChessPiece* PieceToTest = nullptr);

	void TagForMovement();

	void TagPinnedPieces(AChessPiece* Piece);

	bool CanAttackPiece(AChessPiece* Piece, AChessPiece* Target);

	bool CanBeMovedOn(ABoardTile* TileToMoveOn, AChessPiece* MovingPiece);

	EBoardTileState CheckMovementOnTile(
		ABoardTile* RequestedTile, AChessPiece* Piece, bool bStraightLine, EBoardTileState OccupiedTileResult = EBoardTileState::Capture, 
		EBoardTileState UnoccupiedTileResult = EBoardTileState::ValidMove, bool bHideAlliedFailure = false);
	EBoardTileState CheckMovementOnTile(
		ABoardTile* RequestedTile, AChessPiece* Piece, EBoardTileState OccupiedTileResult = EBoardTileState::Capture,
		EBoardTileState UnoccupiedTileResult = EBoardTileState::ValidMove, bool bHideAlliedFailure = false);

	bool IsInStraightLine(FIntVector TileToTest, FIntVector StartTile, FIntVector EndTile);

	void Move(ABoardTile* StartTile, ABoardTile* EndTile, bool bMovingEnPassant = false);

	TMap<FIntVector, EBoardTileState> TestPawnMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestRookMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestBishopMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestKingMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestKnightMovement(FIntVector Position, AChessPiece* Piece);
	TMap<FIntVector, EBoardTileState> TestCastlingMovement(FIntVector Position, AChessPiece* Piece);

	///Tests for Check, Checkmate and Stalemate.
	void TestForGameStatus();

	void StorePiece(AChessPiece* PieceToStore);

	void RemovePiece(AChessPiece* PieceToRemove);
	
	TArray<AChessPiece*> GetTeamPieces(bool bBlackTeam);

	void ClearPinned(EPieceFamilyType FamilyToIgnore = EPieceFamilyType::None);

	void SetTeamInCheck(UTeamPieces* Team, bool bNewCheckStatus);
};
