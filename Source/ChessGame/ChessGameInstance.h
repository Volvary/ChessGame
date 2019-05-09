// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameClasses/Pieces/PieceFamilyType.h"

#include "ChessGameInstance.generated.h"


class APieceFamily;

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText PlayerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	APieceFamily* PlayerFamily = nullptr;

	FPlayerInfo(){}

	FPlayerInfo(FText Name, APieceFamily* Family)
	{
		PlayerName = Name;
		PlayerFamily = Family;
	}
};

/**
 * Used to travel Game Settings to the Game Map.
 */
UCLASS()
class CHESSGAME_API UChessGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pieces")
	TMap<EPieceFamilyType, TSubclassOf<APieceFamily>> AvailableFamilies;

public:

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	TArray<FPlayerInfo> PlayerList;
	
	UFUNCTION(BlueprintCallable)
	void AddToPlayerList(FText PlayerName, TSubclassOf<APieceFamily> FamilyType);

};