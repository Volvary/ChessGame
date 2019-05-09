// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStatusInterface.h"

#include "ChessHUD.h"

void UGameStatusInterface::SetHUD(AChessHUD* NewHUD)
{
	HUD = NewHUD;
}
