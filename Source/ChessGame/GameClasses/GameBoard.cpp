// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBoard.h"
#include "ChessGame.h"
#include "BoardTile.h"
#include "Engine/World.h"

#include "Pieces/ChessPiece.h"
#include "Pieces/PieceDefinitions/PawnPiece.h"
#include "Pieces/PieceDefinitions/RookPiece.h"
#include "Pieces/PieceDefinitions/KnightPiece.h"
#include "Pieces/PieceDefinitions/BishopPiece.h"
#include "Pieces/PieceDefinitions/QueenPiece.h"
#include "Pieces/PieceDefinitions/KingPiece.h"

#include "Pieces/PieceFamily.h"

#include "Generic/BoardActionInformations.h"
#include "Generic/GameResult.h"

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

	TArray<APieceFamily*> FamiliesToUse = GameMode->GetPieceFamiliesToUse();
	APieceFamily* Family = nullptr;

	if (FamiliesToUse.Num() != 2)
	{
		UE_LOG(LogBoard, Error, TEXT("Games other than 1v1 are not yet supported. Aborting..."));
		return;
	}

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

		Family = (i >= FinalBoardSpan * 0.5f) ? FamiliesToUse[0] : FamiliesToUse[1];

		TSubclassOf<AChessPiece> PieceToUse = nullptr;

		switch (GameBoardPieces[i])
		{
		case 1:
			PieceToUse = Family->PawnPiece;			break;
		case 2:
			PieceToUse = Family->RookPiece;			break;
		case 3:
			PieceToUse = Family->KnightPiece;		break;
		case 4:
			PieceToUse = Family->BishopPiece;		break;
		case 5:
			PieceToUse = Family->QueenPiece;		break;
		case 6:
			PieceToUse = Family->KingPiece;			break;
		default:
			break;
		}

		if (PieceToUse != nullptr)
		{
			AChessPiece* Temp = GetWorld()->SpawnActor<AChessPiece>(PieceToUse);

			Temp->SetPieceFamily(Family->FamilyType);
			Temp->SetTeam((i >= FinalBoardSpan * 0.5f) ? true : false);

			StorePiece(Temp);
			GameBoard[i]->SetPiece(Temp);
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

void AGameBoard::ClickedOnTile(ABoardTile* ClickedTile)
{
	if (ClickedTile == nullptr)
	{
		UE_LOG(LogBoard, Error, TEXT("ClickedOnTile was called but ClickedTile was nullptr. Did the tile return itself?"));
		return;
	}

	//Reset Potential Movement just in case.
	PotentialMovement = nullptr;

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
			if (SelectedTilePiece->GetPieceFamily() == GameMode->GetActivePlayer())
			{
				//A piece of the correct player was selected, start confirming movement.
				EBoardTileState* WantedTileState = MarkedTiles.Find(ClickedTile);

				if (WantedTileState != nullptr)
				{
					EBoardTileState TileState = *WantedTileState;

					if (TileState == EBoardTileState::Capture || TileState == EBoardTileState::ValidMove)
					{
						if (TileState == EBoardTileState::Capture && ClickedTile->GetPiece() == nullptr)
						{
							Move(SelectedTile, ClickedTile, true);
						}
						else
						{
							Move(SelectedTile, ClickedTile);
						}

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
		TagForMovement();

		return;
	}
}

TMap<FIntVector, EBoardTileState> AGameBoard::FindValidMoves(AChessPiece* PieceToTest)
{

	TMap<FIntVector, EBoardTileState> ValidCoordinates;

	ABoardTile* TileToTest;

	if (PieceToTest == nullptr)
	{
		if (SelectedTile == nullptr)
		{
			UE_LOG(LogBoard, Error, TEXT("FindValidMoves called but SelectedTile was nullptr. Aborting..."));
			return ValidCoordinates;
		}

		TileToTest = SelectedTile;
	}
	else
	{
		TileToTest = PieceToTest->GetTile();
	}

	

	AChessPiece* Piece = TileToTest->GetPiece();

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
					ValidCoordinates.Append(TestPawnMovement(TileToTest->GetPosition(), Piece));
					break;
				case EPieceMovement::King:
					ValidCoordinates.Append(TestKingMovement(TileToTest->GetPosition(), Piece));
					break;
				case EPieceMovement::Bishop:
					ValidCoordinates.Append(TestBishopMovement(TileToTest->GetPosition(), Piece));
					break;
				case EPieceMovement::Rook:
					ValidCoordinates.Append(TestRookMovement(TileToTest->GetPosition(), Piece));
					break;
				case EPieceMovement::Knight:
					ValidCoordinates.Append(TestKnightMovement(TileToTest->GetPosition(), Piece));
					break;
				case EPieceMovement::Castling:
					ValidCoordinates.Append(TestCastlingMovement(TileToTest->GetPosition(), Piece));
					break;
				}
			}
		}
	}
	else
	{
		ValidCoordinates.Add(TileToTest->GetPosition(), EBoardTileState::Failed);
	}

	TArray<FIntVector> CoordinatesKeys;
	ValidCoordinates.GetKeys(CoordinatesKeys);

	return ValidCoordinates;
}

void AGameBoard::TagForMovement()
{
	TMap<FIntVector, EBoardTileState> ValidCoordinates = FindValidMoves();

	TArray<FIntVector> CoordinatesKeys;
	ValidCoordinates.GetKeys(CoordinatesKeys);

	TArray<FMove> ValidMoves;

	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->Family == GameMode->GetActivePlayer())
		{
			ValidMoves = Team->ValidMoves;
		}
	}

	for (FIntVector Coordinate : CoordinatesKeys)
	{
		ABoardTile* FoundTile = GetTileAt(Coordinate);
		if (FoundTile != nullptr)
		{
			if (ValidMoves.Num() == 0 || ValidMoves.Find(FMove(SelectedTile->GetPiece(), SelectedTile->GetPosition(), Coordinate)) != INDEX_NONE)
			{
				EBoardTileState State = *ValidCoordinates.Find(Coordinate);

				FoundTile->SetTileSelectionState(State);
				MarkedTiles.Add(FoundTile, State);
			}
		}
	}
}

void AGameBoard::TagPinnedPieces(AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> ValidCoordinates = FindValidMoves(Piece);

	TArray<FIntVector> CoordinatesKeys;
	ValidCoordinates.GetKeys(CoordinatesKeys);

	for (FIntVector Coordinate : CoordinatesKeys)
	{
		ABoardTile* FoundTile = GetTileAt(Coordinate);
		if (FoundTile != nullptr)
		{
			EBoardTileState State = *ValidCoordinates.Find(Coordinate);

			if (State == EBoardTileState::Capture)
			{
				FoundTile->GetPiece()->Pin(Piece);
			}
		}
	}
}

bool AGameBoard::CanAttackPiece(AChessPiece* Piece, AChessPiece* Target)
{
	TMap<FIntVector, EBoardTileState> ThreatenedTiles = FindValidMoves(Piece);
	TArray<FIntVector> Keys;
	ThreatenedTiles.GetKeys(Keys);

	for (FIntVector Key : Keys)
	{
		ABoardTile* FoundTile = GetTileAt(Key);

		if (FoundTile != nullptr && FoundTile->GetPiece() == Target)
		{
			EBoardTileState State = *ThreatenedTiles.Find(Key);

			if (State == EBoardTileState::Capture)
			{
				return true;
			}
		}
	}

	return false;
}

bool AGameBoard::CanBeMovedOn(ABoardTile* TileToMoveOn, AChessPiece* MovingPiece)
{
	if (TileToMoveOn != nullptr)
	{
		AChessPiece* RequestedTilePiece = TileToMoveOn->GetPiece();

		//If we are testing a Movement, modify the RequestTilePiece.
		if (PotentialMovement != nullptr)
		{
			//If we are looking at the tile where the piece would be moving FROM, the tile will be empty.
			//If we look at the tile the move goes TO, the tile is the piece moving.
			if (PotentialMovement->StartTile == TileToMoveOn->GetPosition())
			{
				RequestedTilePiece = nullptr;
			}
			else if (PotentialMovement->EndTile == TileToMoveOn->GetPosition())
			{
				RequestedTilePiece = PotentialMovement->Piece;
			}
		}

		//If either there is no piece on the tile or both pieces are not in the same team, it is a valid space.
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

EBoardTileState AGameBoard::CheckMovementOnTile(
	ABoardTile* RequestedTile, AChessPiece* Piece, EBoardTileState OccupiedTileResult, EBoardTileState UnoccupiedTileResult,
	bool bHideAlliedFailure)
{
	//Call the other version of the function, introducing the bStraightLine.
	return CheckMovementOnTile(RequestedTile, Piece, false, OccupiedTileResult, UnoccupiedTileResult, bHideAlliedFailure);
}

#pragma optimize("",off)
EBoardTileState AGameBoard::CheckMovementOnTile(
	ABoardTile* RequestedTile, AChessPiece* Piece, bool bStraightLine, EBoardTileState OccupiedTileResult, EBoardTileState UnoccupiedTileResult, 
	bool bHideAlliedFailure)
{
	if (RequestedTile == nullptr)
	{
		return EBoardTileState::None;
	}

	if (CanBeMovedOn(RequestedTile, Piece))
	{
		bool bIgnorePiece = false;
		AChessPiece* OtherPiece = nullptr;
		FIntVector RequestedPosition = RequestedTile->GetPosition();

		if (PotentialMovement != nullptr && (PotentialMovement->StartTile == RequestedPosition || PotentialMovement->EndTile == RequestedPosition))
		{
			if (bStraightLine && IsInStraightLine(PotentialMovement->EndTile, Piece->GetTile()->GetPosition(), PotentialMovement->StartTile))
			{
				bIgnorePiece = false;
			}
			else
			{
				//Ignore the Piece since it would move out of the way.
				bIgnorePiece = true;
			}
		}
		
		if(!bIgnorePiece)
		{
			OtherPiece = RequestedTile->GetPiece();
		}
		
		if (OtherPiece != nullptr)
		{
			if (bHideAlliedFailure && OtherPiece->IsBlackTeam() == Piece->IsBlackTeam())
			{
				return EBoardTileState::None;
			}
			else
			{
				return OccupiedTileResult;
			}
		}
		else
		{
			return UnoccupiedTileResult;
		}
	}
	else
	{
		return EBoardTileState::Failed;
	}

	return EBoardTileState::None;
}
#pragma optimize("",on)
///Auto-generated VA pragma optimize for debug.

#pragma optimize("",off)
bool AGameBoard::IsInStraightLine(FIntVector TileToTest, FIntVector StartTile, FIntVector EndTile)
{
	FIntVector Direction = StartTile - EndTile;
	int FoundValue = -13337;

	if (StartTile == EndTile)
	{
		//Same tile is not a line.
		return false;
	}

	for (int i = 0; i < Direction.Num(); i++)
	{
		if (Direction[i] != 0)
		{
			if (FoundValue == -13337 || FMath::Abs(Direction[i]) == FoundValue)
			{
				FoundValue = FMath::Abs(Direction[i]);
			}
			else
			{
				//A second value has been found and a straight line cannot be established.
				return false;
			}
		}
	}

	FIntVector TargetDirection = StartTile - TileToTest;

	int TargetSize = FMath::Abs(TargetDirection.GetMin()) > TargetDirection.GetMax() ? TargetDirection.GetMin() : TargetDirection.GetMax();
	int DirectionSize = FMath::Abs(Direction.GetMin()) > Direction.GetMax() ? Direction.GetMin() : Direction.GetMax();
	//If both unit vectors are equal, then they are in the same straight line.
	if (TargetSize != 0 && DirectionSize != 0 && TargetDirection / TargetSize == Direction / DirectionSize)
	{
		return true;
	}

	return false;
}
#pragma optimize("",on)
///Auto-generated VA pragma optimize for debug.

void AGameBoard::Move(ABoardTile* StartTile, ABoardTile* EndTile, bool bMovingEnPassant)
{
	if (StartTile == nullptr || EndTile == nullptr)
	{
		UE_LOG(LogBoard, Error, TEXT("Move was called but one of the params was nullptr. Expecting valid info. Canceling."));
		return;
	}

	AChessPiece* CapturedPiece = nullptr;
	if (bMovingEnPassant)
	{
		//If the piece is moving using a En Passant attack, the captured piece is not on the EndTile but rather in the MostRecentMove.
		CapturedPiece = MostRecentMove;
	}
	else
	{
		CapturedPiece = EndTile->GetPiece();
	}

	if (CapturedPiece != nullptr)
	{
		RemovePiece(CapturedPiece);

		CapturedPiece->Destroy();
		EndTile->EmptyPiece();
	}

	AChessPiece* MovingPiece = StartTile->GetPiece();
	MovingPiece->SetHasMoved(true);
	EndTile->SetPiece(MovingPiece);
	StartTile->EmptyPiece();

	if (MostRecentMove != nullptr)
	{
		MostRecentMove->SetEnPassant(false);
	}
	MostRecentMove = MovingPiece;

	bool bWaitForPromotion = false;

	if ((bool) (MovingPiece->GetAvailableMovement() & EPieceMovement::Pawn))
	{
		//If a pawn has reached the other end of the board, promote it.
		if (EndTile->GetPosition().X == 0 || EndTile->GetPosition().X == GameBoardSize - 1)
		{
			//GameMode->ShowPromotionScreen();
			bWaitForPromotion = true;
		}

		//If a pawn moved two squares, set it En Passant vulnerable.
		
		FIntVector Distance = StartTile->GetPosition() - EndTile->GetPosition();
		if (FMath::Abs(Distance.X) == 2)
		{
			MovingPiece->SetEnPassant(true);
		}
	}

	SelectTile(nullptr);

	if (!bWaitForPromotion)
	{
		TestForGameStatus();
	}
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestPawnMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;
	bool bMoveBlocked = false;

	int Direction = Piece->IsBlackTeam() ? -1 : 1;

	ABoardTile* RequestedTile;
	EBoardTileState TileResult;
	
	//First test moving forward.
	RequestedTile = GetTileAt(Position.X + Direction, Position.Y);
	TileResult = CheckMovementOnTile(RequestedTile, Piece, EBoardTileState::Failed, EBoardTileState::ValidMove);
	
	if (TileResult != EBoardTileState::None)
	{
		Out.Add(RequestedTile->GetPosition(), TileResult);
		if (TileResult == EBoardTileState::Failed || TileResult == EBoardTileState::Capture)
		{
			bMoveBlocked = true;
		}
	}

	//Then test the sides, hiding any failures.
	for (int i = -1; i < 2; i++)
	{
		if (i == 0)
		{
			continue;
		}

		RequestedTile = GetTileAt(Position.X + Direction, Position.Y + i);
		TileResult = CheckMovementOnTile(RequestedTile, Piece, EBoardTileState::Capture, EBoardTileState::ValidMove, true);

		if (TileResult == EBoardTileState::Capture)
		{
			Out.Add(RequestedTile->GetPosition(), TileResult);
		}
		else if (TileResult == EBoardTileState::ValidMove)
		{
			//If the tile ahead is clear, test the tile next to the piece to see if it contains a piece weak to En Passant.
			ABoardTile* TemporaryTile = GetTileAt(Position.X, Position.Y + i);
			EBoardTileState TemporaryResult = CheckMovementOnTile(TemporaryTile, Piece, EBoardTileState::Capture, EBoardTileState::None);

			if (TemporaryResult == EBoardTileState::Capture && TemporaryTile->GetPiece()->CanBeEnPassant() == true)
			{
				//Tag the RequestedTile regardless as it will be the tile the pawn will move to.
				Out.Add(RequestedTile->GetPosition(), TemporaryResult);
			}
		}
	}

	//If the move was not blocked, check for two forward.
	if (!bMoveBlocked && !Piece->HasMoved()) 
	{
		RequestedTile = GetTileAt(Position.X + Direction * 2,Position.Y);
		TileResult = CheckMovementOnTile(RequestedTile, Piece, EBoardTileState::Failed, EBoardTileState::ValidMove);

		if (TileResult != EBoardTileState::None)
		{
			Out.Add(RequestedTile->GetPosition(), TileResult);
			
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

	//For every orthogonal direction, test until something is hit.
	for (FIntVector Direction : Directions)
	{
		int i = 1;
		while (i < GameBoardSize && i > -1)
		{
			ABoardTile* RequestedTile;
			RequestedTile = GetTileAt(Position + (Direction * i));

			EBoardTileState TileResult = CheckMovementOnTile(RequestedTile, Piece, true);
			
			if (TileResult != EBoardTileState::None)
			{
				Out.Add(RequestedTile->GetPosition(), TileResult);
				if (TileResult == EBoardTileState::Failed || TileResult == EBoardTileState::Capture)
				{
					break;
				}
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

	//For every diagonal direction, test until something is hit.
	for (FIntVector Direction : Directions)
	{
		int i = 1;
		while (i < GameBoardSize && i > -1)
		{
			ABoardTile* RequestedTile;
			RequestedTile = GetTileAt(Position + (Direction * i));

			EBoardTileState TileResult = CheckMovementOnTile(RequestedTile, Piece, true);

			if (TileResult != EBoardTileState::None)
			{
				Out.Add(RequestedTile->GetPosition(), TileResult);
				if (TileResult == EBoardTileState::Failed || TileResult == EBoardTileState::Capture)
				{
					break;
				}
			}

			i++;
		}
	}

	return Out;
}

TMap<FIntVector, EBoardTileState> AGameBoard::TestKingMovement(FIntVector Position, AChessPiece* Piece)
{
	TMap<FIntVector, EBoardTileState> Out;

	//Test every tile around the Royal piece. Test for checkmate done later.
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

			EBoardTileState TileResult = CheckMovementOnTile(RequestedTile,Piece);
			
			if (TileResult != EBoardTileState::None)
			{
				Out.Add(RequestedTile->GetPosition(), TileResult);
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

		EBoardTileState TileResult = CheckMovementOnTile(RequestedTile, Piece);

		if (TileResult != EBoardTileState::None)
		{
			Out.Add(RequestedTile->GetPosition(), TileResult);
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

#pragma optimize("",off)
void AGameBoard::TestForGameStatus()
{
	//Verify Draw
	bool bOnlyRoyalAlive = true;

	for (UTeamPieces* Team : GamePieces)
	{
		//End early if even a single not royal is found.
		if (!bOnlyRoyalAlive)
		{
			break;
		}

		for (AChessPiece* Piece : Team->TeamPieces)
		{
			if(!Piece->IsRoyal())
			{
				bOnlyRoyalAlive = false;
				break;
			}
		}
	}

	if (bOnlyRoyalAlive)
	{
		GameMode->EndGame(EGameResult::Draw);
		return;
	}

	//Confirm for Check

	EPieceFamilyType ActiveTeam = GameMode->GetActivePlayer();

	ClearPinned(ActiveTeam);

	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->Family == ActiveTeam)
		{
			for (AChessPiece* Piece : Team->TeamPieces)
			{
				TagPinnedPieces(Piece);
			}
		}
	}

	UTeamPieces* TeamInCheck = nullptr;
	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->Family != ActiveTeam)
		{
			for (AChessPiece* Piece : Team->TeamPieces)
			{
				if (Piece->IsRoyal() == true && Piece->IsPinned())
				{
					SetTeamInCheck(Team, true);
					TeamInCheck = Team;
				}
			}
		}
	}

	UTeamPieces* NextPlayer = nullptr;

	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->Family == GameMode->GetNextPlayer())
		{
			NextPlayer = Team;
			break;
		}
	}

	if(NextPlayer != nullptr){

		AChessPiece* ThreatenedRoyalPiece = nullptr;
		TArray<FIntVector> RoyalPieceLocations;

		TArray<AChessPiece*> EnemyPieces;
		TArray<FMove> PotentialMovements;	//Used to store Friendly moves that could be done.
		TArray<FMove> ValidMovements;		//Used to store validated moves.

		TMap<FIntVector, EBoardTileState> PieceMovements;	//Contains the tiles a piece can move to.
		TArray<FIntVector> Keys;
		EBoardTileState* TileStatus;

		PotentialMovement = nullptr;
		
		for (UTeamPieces* Team : GamePieces)
		{
			if (Team->Family != NextPlayer->Family)
			{
				EnemyPieces.Append(Team->TeamPieces);
			}
		}

		//Get the threatened royal piece.
		for (AChessPiece* Piece : NextPlayer->TeamPieces)
		{
			if (Piece->IsRoyal())
			{
				
				if (Piece->IsPinned())
				{
					//If more than one royal piece is pinned, player is defeated.
					if(ThreatenedRoyalPiece != nullptr)
					{
						GameMode->EndGame(EGameResult::Checkmate);
						return;
					}

					ThreatenedRoyalPiece = Piece;
				}

					RoyalPieceLocations.Add(Piece->GetPosition());
			}
		}

		//Obtain all the potential movements.
		for (AChessPiece* Piece : NextPlayer->TeamPieces)
		{
			PieceMovements = FindValidMoves(Piece);
			Keys.Empty();
			PieceMovements.GetKeys(Keys);

			for (FIntVector Tile : Keys)
			{
				TileStatus = PieceMovements.Find(Tile);

				if (TileStatus != nullptr && (*TileStatus == EBoardTileState::ValidMove || *TileStatus == EBoardTileState::Capture) )
				{
					PotentialMovements.Add(FMove(Piece, Piece->GetPosition(), Tile));
				}
			}
		}

		//for every movement, analyze whether it would put a royal piece at risk.
		for (FMove Movement : PotentialMovements)
		{
			bool bValidMove = true;

			//Assign the current movement to PotentialMovement, so CanMoveOnTile apply it.
			PotentialMovement = &Movement;

			for(AChessPiece* EnemyPiece : EnemyPieces)
			{
				//If the current movement would kill this piece, ignore the piece.
				if (Movement.EndTile == EnemyPiece->GetPosition())
				{
					continue;
				}

				PieceMovements = FindValidMoves(EnemyPiece);
				Keys.Empty();
				PieceMovements.GetKeys(Keys);

				for (FIntVector Tile : Keys)
				{
					//If any of the Royal Piece is in range of this piece, move is invalid and we move on to the next piece.
					if (RoyalPieceLocations.Find(Tile) != INDEX_NONE)
					{
						bValidMove = false;
						break;
					}
				}

				//Stop testing if one tile already invalidated the move.
				if (!bValidMove)
				{
					break;
				}
			}

			if (bValidMove)
			{
				ValidMovements.Add(Movement);
			}
		}

		//If no movements could be found without threatening the royal,
		if (ValidMovements.Num() == 0)
		{
			//If the royal pieces are threathened, Checkmate. Else, Stalemate.
			if (TeamInCheck == NextPlayer)
			{
				GameMode->EndGame(EGameResult::Checkmate);
			}
			else
			{
				GameMode->EndGame(EGameResult::Stalemate);
			}
			return;
		}
		else
		{
			NextPlayer->ValidMoves = ValidMovements;
		}

		//Reset PotentialMovement to nullptr before proceeding.
		PotentialMovement = nullptr;

	}

	GameMode->PlayerFinishedPlaying();
}
#pragma optimize("",on)
///Auto-generated VA pragma optimize for debug.
void AGameBoard::StorePiece(AChessPiece* PieceToStore)
{
	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->TeamPieces.Num() != 0 && Team->Family == PieceToStore->GetPieceFamily())
		{
			Team->TeamPieces.Add(PieceToStore);
			return;
		}
	}

	//If no team was found for the piece, create a new Team->

	UTeamPieces* NewTeam = NewObject<UTeamPieces>(UTeamPieces::StaticClass());
	if (NewTeam != nullptr)
	{
		NewTeam->TeamPieces.Add(PieceToStore);
		NewTeam->Family = PieceToStore->GetPieceFamily();
		GamePieces.Add(NewTeam);
	}
	else
	{
		UE_LOG(LogBoard, Error, TEXT("A Team was supposed to be spawned in StorePiece but failed."));
	}
}

void AGameBoard::RemovePiece(AChessPiece* PieceToRemove)
{
	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->TeamPieces.Find(PieceToRemove) != INDEX_NONE)
		{
			Team->TeamPieces.Remove(PieceToRemove);
			return;
		}
	}
}

TArray<AChessPiece*> AGameBoard::GetTeamPieces(bool bBlackTeam)
{
	TArray<AChessPiece*> Out;

	//Check each team to find the proper array then return the proper one.
	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->TeamPieces.Num() != 0 && Team->TeamPieces[0]->IsBlackTeam() == bBlackTeam)
		{
			return Team->TeamPieces;
		}
	}

	return Out;
}

void AGameBoard::ClearPinned(EPieceFamilyType FamilyToIgnore /*= EPieceFamilyType::None*/)
{
	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->Family == FamilyToIgnore)
		{
			continue;
		}

		for (AChessPiece* Piece : Team->TeamPieces)
		{
			if (Piece != nullptr)
			{
				Piece->ClearPins();
			}
		}
	}
}

void AGameBoard::SetTeamInCheck(UTeamPieces* InTeam, bool bNewCheckStatus)
{
	InTeam->bInCheck = bNewCheckStatus;

	for (UTeamPieces* Team : GamePieces)
	{
		if (Team->bInCheck)
		{
			//GameMode->ToggleCheckVisualForPlayers();
			return;
		}
	}
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
	MarkedTiles.Empty();

	SelectedTile = NewSelected;
}