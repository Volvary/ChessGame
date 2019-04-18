// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnPromotionInterface.h"

#include "GameClasses/InGameGM.h"

void UPawnPromotionInterface::ChoosePromotion(EPieceType Promotion)
{
	//If moving onto multiplayer, pass via the L-PlayerController -> S-PlayerController -> GameMode pathway.
	GameMode->PromotePiece(Promotion);
}

void UPawnPromotionInterface::SetGameMode(AInGameGM* InGameMode)
{
	GameMode = InGameMode;
}
