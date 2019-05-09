// Fill out your copyright notice in the Description page of Project Settings.

#include "SettingsMenu.h"

#include "ChessHUD.h"
#include "GameClasses/InGamePlayerController.h"

void USettingsMenu::SetHUD(AChessHUD* NewHUD)
{
	HUD = NewHUD;
}

void USettingsMenu::UpdatePlayer1Information(const FText& NewPlayerName, EPieceFamilyType NewPieceFamily)
{
	if(HUD != nullptr && HUD->GetOwningPlayerController() != nullptr)

	AInGamePlayerController* PC = Cast<AInGamePlayerController>(HUD->GetOwningPlayerController());
}

void USettingsMenu::UpdatePlayer2Information(const FText& NewPlayerName, EPieceFamilyType NewPieceFamily)
{

}
