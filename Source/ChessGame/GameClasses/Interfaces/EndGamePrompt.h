// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Generic/GameResult.h"

#include "EndGamePrompt.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API UEndGamePrompt : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintNativeEvent)
	void SetGameResult(EGameResult Result, const FText& Winner);
};
