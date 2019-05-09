// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GameStatusInterface.generated.h"

class AChessHUD;

/**
 * 
 */
UCLASS()
class CHESSGAME_API UGameStatusInterface : public UUserWidget
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(BlueprintReadOnly)
	AChessHUD* HUD = nullptr;

public:

	void SetHUD(AChessHUD* NewHUD);
};
