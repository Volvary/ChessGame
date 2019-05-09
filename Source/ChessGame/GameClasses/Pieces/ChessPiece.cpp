// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessPiece.h"
#include "GameClasses/BoardTile.h"
#include "AkAudio/Classes/AkComponent.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AChessPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessPiece::SetPieceFamily(EPieceFamilyType Family)
{
	PieceFamily = Family;
}

void AChessPiece::ClearPins()
{
	PinningPieces.Empty();
}

ABoardTile* AChessPiece::GetTile()
{
	return CurrentTile;
}

void AChessPiece::SetCurrentTile(ABoardTile* Tile)
{
	CurrentTile = Tile;
}

FIntVector AChessPiece::GetPosition()
{
	if (CurrentTile != nullptr)
	{
		return CurrentTile->GetPosition();
	}

	return FIntVector(-1,-1,-1);
}

void AChessPiece::Pin(AChessPiece* PinningPiece)
{
	PinningPieces.Add(PinningPiece);
}

TArray<AChessPiece*> AChessPiece::GetPinningPieces()
{
	return PinningPieces;
}

void AChessPiece::ChangeColor_Implementation()
{

}

