// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Generic/BoardActionInformations.h"

#include "BoardTile.generated.h"

class AChessPiece;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileClicked, ABoardTile*, ClickedTile);

UCLASS()
class CHESSGAME_API ABoardTile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TileSize;

	UPROPERTY(BlueprintReadOnly, BlueprintCallable)
	FTileClicked OnTileClicked;

protected:
	UPROPERTY(BlueprintReadOnly)
	FIntVector Position;

	UPROPERTY(BlueprintReadOnly)
	int GameBoardSize;

	UPROPERTY(Transient)
	AChessPiece* CurrentPiece = nullptr;

	UPROPERTY(BlueprintReadOnly)
	EBoardTileState SelectionState = EBoardTileState::None;
	
public:	
	// Sets default values for this actor's properties
	ABoardTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ChangeColor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPositionOnBoard(int x, int y, int BoardSize);

	bool SetPiece(AChessPiece* NewPiece);

	void EmptyPiece();

	EBoardTileState GetTileSelectionState() { return SelectionState; }

	void SetTileSelectionState(EBoardTileState NewState) { SelectionState = NewState; ChangeColor();}

	AChessPiece* GetPiece();

	FIntVector GetPosition() {return Position;}
};
