// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GameClasses/Pieces/PieceFamilyType.h"

#include "SettingsMenu.generated.h"

class AChessHUD;

/**
 * 
 */
UCLASS()
class CHESSGAME_API USettingsMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	AChessHUD* HUD;

	EPieceFamilyType Player1FamilyType = EPieceFamilyType::PlaceholderWhite;
	EPieceFamilyType Player2FamilyType = EPieceFamilyType::PlaceholderBlack;

public:

	void SetHUD(AChessHUD* NewHUD);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayer1Information(const FText& NewPlayerName, EPieceFamilyType NewPieceFamily);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayer2Information(const FText& NewPlayerName, EPieceFamilyType NewPieceFamily);

};
