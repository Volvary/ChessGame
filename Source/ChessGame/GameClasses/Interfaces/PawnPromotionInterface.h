// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GameClasses/Pieces/PieceMovement.h"

#include "PawnPromotionInterface.generated.h"

class AInGameGM;

/**
 * 
 */
UCLASS()
class CHESSGAME_API UPawnPromotionInterface : public UUserWidget
{
	GENERATED_BODY()
	
	AInGameGM* GameMode = nullptr;

public:

	UFUNCTION(BlueprintCallable)
	void ChoosePromotion(EPieceType Promotion);

	void SetGameMode(AInGameGM* InGameMode);
};
