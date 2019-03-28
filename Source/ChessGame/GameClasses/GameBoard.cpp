// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBoard.h"
#include "ChessGame.h"
#include "BoardTile.h"
#include "Engine/World.h"

#include "Pieces/ChessPiece.h"
#include "Pieces/PawnPiece.h"
#include "Pieces/RookPiece.h"
#include "Pieces/KnightPiece.h"
#include "Pieces/BishopPiece.h"
#include "Pieces/QueenPiece.h"
#include "Pieces/KingPiece.h"

#include "Generic/BoardActionInformations.h"

#include "InGameGM.h"

// Sets default values
AGameBoard::AGameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameBoard::BeginPlay()
{
	Super::BeginPlay();

	PrepareGameBoard();
	
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameBoard::SetGameMode(AInGameGM* GM)
{
	GameMode = GM;
}

void AGameBoard::PrepareGameBoard()
{
	if (BoardTileSubclass != nullptr)
	{
		ABoardTile* TileTemp = Cast<ABoardTile>(BoardTileSubclass->ClassDefaultObject);
		if (TileTemp != nullptr)
		{
			TileSize = TileTemp->TileSize;
		}
	}

	int GameBoardPieces[] = {	
		2,3,4,5,6,4,3,2,
		1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,
		2,3,4,5,6,4,3,2
	};

	GameBoardDimensions = 2;
	GameBoardSize = 8;

	//With a classic game board, this will give 8^2 = 64.
	int FinalBoardSpan = FMath::Pow(GameBoardSize, GameBoardDimensions); 

	for (int i = 0; i < FinalBoardSpan; i++)
	{
		FIntVector BoardPosition = FIntVector(i / GameBoardSize, i % GameBoardSize, 0);

		//Convert the IntVector into Float then multiply by the piece's size.
		FVector WorldPosition = FVector(BoardPosition) * TileSize;

		ABoardTile* NewTile = GetWorld()->SpawnActor<ABoardTile>(BoardTileSubclass, WorldPosition, FRotator::ZeroRotator);
		
		if (NewTile != nullptr)
		{
			NewTile->SetPositionOnBoard(BoardPosition.X, BoardPosition.Y, GameBoardSize);
			NewTile->OnTileClicked.AddDynamic(this, &AGameBoard::ClickedOnTile);
		}

		GameBoard.Add(NewTile);

		TSubclassOf<AChessPiece> PieceToUse = nullptr;

		switch (GameBoardPieces[i])
		{
		case 1:
			PieceToUse = PawnSubclass;			break;
		case 2:
			PieceToUse = RookSubclass;			break;
		case 3:
			PieceToUse = KnightSubclass;		break;
		case 4:
			PieceToUse = BishopSubclass;		break;
		case 5:
			PieceToUse = QueenSubclass;			break;
		case 6:
			PieceToUse = KingSubclass;			break;
		default:
			break;
		}

		if (PieceToUse != nullptr)
		{
			AChessPiece* Temp = GetWorld()->SpawnActor<AChessPiece>(PieceToUse);
			GamePieces.Add(Temp);
			GameBoard[i]->SetPiece(Temp);

			Temp->SetTeam(i >= FinalBoardSpan * 0.5f);
		}
	}
}

ABoardTile* AGameBoard::GetTileAt(int X, int Y, int Z /*= 0*/)
{
	if (X < 0 || X > GameBoardSize - 1 || Y < 0 || Y > GameBoardSize - 1)
	{
		return nullptr;
	}

	int RequestedIndex = X * GameBoardSize + Y;
	ABoardTile* RequestedTile = nullptr;

	//Confirm that RequestedIndex makes sense.
	if (RequestedIndex < GameBoard.Num() && RequestedIndex > -1)
	{
		RequestedTile = GameBoard[RequestedIndex];
	}

	return RequestedTile;
}

ABoardTile* AGameBoard::GetTileAt(FIntVector Position)
{
	return GetTileAt(Position.X, Position.Y, Position.Z);
}
#pragma optimize("",off)

void AGameBoard::ClickedOnTile(ABoardTile* ClickedTile)
{
	if (ClickedTile == nullptr)
	{
		UE_LOG(LogBoard, Error, TEXT("ClickedOnTile was called but ClickedTile was nullptr. Did the tile return itself?"));
		return;
	}
	//If Selected Tile is the same as ClickedTile, mark it as cleared.
	if (SelectedTile == ClickedTile)
	{
		SelectTile(nullptr);

		return;
	}

	//If a Tile is selected, either test for validity or clear the tile then proceed.
	if (SelectedTile != nullptr)
	{
		AChessPiece* SelectedTilePiece = SelectedTile->GetPiece();
		if (SelectedTilePiece == nullptr)
		{
			//The Selected Tile was empty and simply marked as Failed. Clear it then continue.

			SelectTile(nullptr);
		}
		else
		{
			if (SelectedTilePiece->IsBlackTeam() == GameMode->IsBlackPlaying())
			{
				//A piece of the correct player was selected, start confirming movement.
				EBoardTileState* WantedTileState = MarkedTiles.Find(ClickedTile);

				if (WantedTileState != nullptr)
				{
					EBoardTileState TileState = *WantedTileState;

					if (TileState == EBoardTileState::Capture || TileState == EBoardTileState::ValidMove)
					{
						Move(SelectedTile, ClickedTile);
						return;
					}
					else
					{
						//Tile was found but was not a proper move for this piece. Clear it then continue.
						SelectTile(nullptr);
					}
				}
			}
			SelectTile(nullptr);
		}
	}

	//If no Tile is currently selected, do first click selection
	if (SelectedTile == nullptr)
	{
		SelectTile(ClickedTile);
		SelectedTile->SetTileSelectionState(EBoardTileState::Clicked);
		FindValidMoves();

		return;
	}
}
#pragma optimize("",on)
///Auto-generated VA pragma optimize for debug.
void AGameBoard::FindValidMoves()
{
	AChessPiece* Piece = SelectedTile->GetPiece();
	TMap<FIntVector, EBoardTileState> ValidCoordinates;

	if (SelectedTile == nullptr)
	{
		UE_LOG(LogBoard, Error, TEXT("FindValidMoves called but SelectedTile was nullptr. Aborting..."));
	}
	if (Piece != nullptr)
	{
		EPieceMovement PieceMovement = Piece->GetAvailableMovement();

		for (uint8 i = 1; i < (uint8)EPieceMovement::MAX_VALUE; i*=2)
		{
			EPieceMovement Test = EPieceMovement(i);

			if ((bool)(PieceMovement & Test))
			{
				switch (Test)
				{
				case EPieceMovement::Pawn:
					ValidCoordinates.Append(TestPawnMovement(SelectedTile->GetPosition(), Piece));
					break;
				case EPieceMovement::King:
					ValidCoordinates.Append(TestKingMovement(SelectedTile->GetPosition(), Piece));
					break;
				case EPieceMovement::Bishop:
					ValidCoordinates.Append(TestBishopMovement(SelectedTile->GetPosition(), Piece));
					break;
				case EPieceMovement::Rook:
					ValidCoordinates.Append(TestRookMovement(SelectedTile->GetPosition(), Piece));
					break;
				case EPieceMovement::Knight:
					ValidCoordinates.Append(TestKnightMovement(SelectedTile->GetPosition(), Piece));
					break;
				case EPieceMovement::Castling:
					ValidCoordinates.Append(TestCastlingMovement(SelectedTile->GetPosition(), Piece));
					break;
				}
			}
		}
	}
	else
	{
		ValidCoordinates.Add(SelectedTile->GetPosition(), EBoardTileState::Failed);
	}

	TArray<FIntVector> CoordinatesKeys;
	ValidCoordinates.GetKeys(CoordinatesKeys);
	for (FIntVector Coordinate : CoordinatesKeys)
	{
		ABoardTile* FoundTile = GetTileAt(Coordinate);
		if (FoundTile != nullptr)
		{
			FoundTile->SetTileSelectionState(*ValidCoordinates.Find(Coordinate));

			MarkedTiles.Add(FoundTile, *ValidCoordinates.Find(Coordinate));
		}
	}
}

bool AGameBoard::CanBeMovedOn(ABoardTile* TileToMoveOn, AChessPiece* MovingPiece)
{
	if (TileToMoveOn != nullptr)
	{
		AChessPiece* RequestedTilePiece = TileToMoveOn->GetPiece();

		//If either there is no piece on the tile or both pieces are not in the same team, add as a valid space.
		if (RequestedTilePiece != nullptr)
		{
			if (RequestedTilePiece->IsBlackTeam() != MovingPiece->IsBlackTeam())
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}

void AGameBoard::Move(ABoardTile* StartTile, ABoardTile* EndTile)
{
	if (StartTile == nullptr || EndTile == nullptr)
	{
		UE_LOG(LogBoard, Error, TEXT("Move was called but one of the params was nullptr. Expecting valid info. Canceling."));
		return;
	}

	AChessPiece* EndTilePiece = EndTile->GetPiece();
	if (EndTilePiece != nullptr)
	{
		GamePieces.Remove(EndTilePiece);

		EndTilePiece->Destroy();
		EndTile->EmptyPiece();
	}

	AChessPiece* MovingPiece = StartTile->GetPiece();
	MovingPiece->SetHasMoved(true);
	EndTile->SetPiece(MovingPiece);
	StartTile->EmptyPiece();

	SelectTile(nullptr);

	GameMode->PlayerFinishedPlaying();
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestPawnMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;
	bool bMoveBlocked = false;

	int Direction = Piece->IsBlackTeam() ? -1 : 1;

	ABoardTile* RequestedTile;
	RequestedTile = GetTileAt(Position.X + Direction,Position.Y);
	if (CanBeMovedOn(RequestedTile, Piece))
	{
		Out.Add(FIntVector(Position.X + Direction,Position.Y, 0), EBoardTileState::ValidMove);
	}
	else
	{
		bMoveBlocked = true;
		Out.Add(FIntVector(Position.X + Direction,Position.Y, 0), EBoardTileState::Failed);
	}

	if (!bMoveBlocked && !Piece->HasMoved()) //If the move was not blocked.
	{
		RequestedTile = GetTileAt(Position.X + Direction * 2,Position.Y);
		if (CanBeMovedOn(RequestedTile, Piece))
		{
			Out.Add(FIntVector(Position.X + Direction * 2, Position.Y, 0), EBoardTileState::ValidMove);
		}
	}

	return Out;
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestRookMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;
	TArray<FIntVector> Directions = {
		FIntVector( 1,0 ,0),
		FIntVector(-1,0 ,0),
		FIntVector(0 ,1 ,0),
		FIntVector(0 ,-1,0)
	};

	for (FIntVector Direction : Directions)
	{
		int i = 1;
		while (i < GameBoardSize && i > -1)
		{
			ABoardTile* RequestedTile;
			RequestedTile = GetTileAt(Position + (Direction * i));

			if (CanBeMovedOn(RequestedTile, Piece))
			{
				Out.Add(Position + (Direction * i), EBoardTileState::ValidMove);
			}
			else
			{
				Out.Add(Position + (Direction * i), EBoardTileState::Failed);
				break;
			}

			i++;
		}
	}

	return Out;
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestBishopMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;
	TArray<FIntVector> Directions = {
		FIntVector(1	,1	,0),
		FIntVector(-1	,1	,0),
		FIntVector(1	,-1	,0),
		FIntVector(-1	,-1	,0)
	};

	for (FIntVector Direction : Directions)
	{
		int i = 1;
		while (i < GameBoardSize && i > -1)
		{
			ABoardTile* RequestedTile;
			RequestedTile = GetTileAt(Position + (Direction * i));

			if (CanBeMovedOn(RequestedTile, Piece))
			{
				Out.Add(Position + (Direction * i), EBoardTileState::ValidMove);
			}
			else
			{
				Out.Add(Position + (Direction * i), EBoardTileState::Failed);
				break;
			}

			i++;
		}
	}

	return Out;
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestKingMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;

	for (int i = -1; i <= 1; i++)
	{
		for (int k = -1; k <= 1; k++)
		{
			if (i == 0 && k == 0)
			{
				continue;
			}

			ABoardTile* RequestedTile;
			RequestedTile = GetTileAt(Position + FIntVector(i,k,0));

			if (CanBeMovedOn(RequestedTile, Piece))
			{
				Out.Add(Position + FIntVector(i,k,0), EBoardTileState::ValidMove);
			}
			else
			{
				Out.Add(Position + FIntVector(i, k, 0), EBoardTileState::Failed);
				continue;
			}
		}
	}

	return Out;
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestKnightMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;
	TArray<FIntVector> Directions = {
		FIntVector(-1	,2	,0),
		FIntVector(1	,2	,0),
		FIntVector(-2	,1	,0),
		FIntVector(2	,1	,0),
		FIntVector(-2	,-1	,0),
		FIntVector(2	,-1	,0),
		FIntVector(-1	,-2	,0),
		FIntVector(1	,-2	,0)
	};

	for (FIntVector Direction : Directions)
	{
		ABoardTile* RequestedTile;
		RequestedTile = GetTileAt(Position + Direction);

		if (CanBeMovedOn(RequestedTile, Piece))
		{
			Out.Add(Position + Direction, EBoardTileState::ValidMove);
		}
		else
		{
			Out.Add(Position + Direction, EBoardTileState::Failed);
			continue;
		}
	}

	return Out;
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestCastlingMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;

	if (!Piece->HasMoved())
	{
		return Out;
	}

	return Out;
}

void AGameBoard::SelectTile(ABoardTile* NewSelected)
{
	if (SelectedTile != nullptr)
	{
		SelectedTile->SetTileSelectionState(EBoardTileState::None);

		TArray<ABoardTile*> Tiles;
		MarkedTiles.GetKeys(Tiles);	
		for (ABoardTile* Tile : Tiles)
		{
			Tile->SetTileSelectionState(EBoardTileState::None);
		}
	}
	SelectedTile = NewSelected;
}