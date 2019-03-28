// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardTile.h"
#include "Pieces/ChessPiece.h"

#include "ChessGame.h"

// Sets default values
ABoardTile::ABoardTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoardTile::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentPiece = nullptr;
}

void ABoardTile::ChangeColor_Implementation()
{

}

// Called every frame
void ABoardTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardTile::SetPositionOnBoard(int x, int y, int BoardSize)
{
	Position.X = x;
	Position.Y = y;

	GameBoardSize = BoardSize;

	ChangeColor();
}

bool ABoardTile::SetPiece(AChessPiece* NewPiece)
{
	if (CurrentPiece != nullptr){
		UE_LOG(LogBoard, Warning, TEXT("To avoid losing a piece, call EmptyPiece to clear this Tile."))
		return false;
	}

	CurrentPiece = NewPiece;
	if (CurrentPiece != nullptr)
	{
		CurrentPiece->SetActorLocation(GetActorLocation() + FVector(TileSize * 0.5f, TileSize * 0.5f, 0.0f));
	}
	return true;
}

void ABoardTile::EmptyPiece()
{
	CurrentPiece = nullptr;
}

AChessPiece* ABoardTile::GetPiece()
{
	return CurrentPiece;
}

