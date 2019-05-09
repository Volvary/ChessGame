// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameClasses/Pieces/PieceFamilyType.h"

#include "Generic/GameResult.h"

#include "InGamePlayerController.generated.h"

class UTurnStatusInterface;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UTurnStatusInterface* TurnStatus;

public:

	void BeginPlay() override;

	void UpdateActivePlayer(const FText& ActivePlayer);

	void ShowEndGamePrompt(EGameResult Result, FText Winner);
};
