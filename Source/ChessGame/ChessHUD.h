// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/SlateWrapperTypes.h"

#include "ChessHUD.generated.h"

class UUserWidget;

class UTurnStatusInterface;
class UGameStatusInterface;
class UPawnPromotionInterface;
class USettingsMenu;
class UQuitMenu;
class UEndGamePrompt;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UTurnStatusInterface> TurnStatusInterfaceClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGameStatusInterface> GameStatusInterfaceClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UPawnPromotionInterface> PawnPromotionInterfaceClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<USettingsMenu> SettingsMenuClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UQuitMenu> QuitMenuClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UEndGamePrompt> EndGamePromptClass = nullptr;

	TMap<TSubclassOf<UUserWidget>, UUserWidget*> InterfaceMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<TSubclassOf<UUserWidget>, int> InterfaceZOrder;

	UUserWidget* ShowInterface(TSubclassOf<UUserWidget> Widget, ESlateVisibility Visibility = ESlateVisibility::Visible);

	void HideInterface(TSubclassOf<UUserWidget> Widget);

	int GetZOrder(TSubclassOf<UUserWidget> Widget);

public:

	UFUNCTION(BlueprintCallable)
	void ShowPromotionInterface();

	void HidePromotionInterface();

	UFUNCTION(BlueprintCallable)
	UTurnStatusInterface* ShowTurnStatusInterface();

	void HideTurnStatusInterface();

	UFUNCTION(BlueprintCallable)
	USettingsMenu* ShowSettingsMenu();

	void HideSettingsMenu();

	UFUNCTION(BlueprintCallable)
	UGameStatusInterface* ShowGameStatusInterface();

	void HideGameStatusInterface();

	UFUNCTION(BlueprintCallable)
	void ShowQuitGameMenu();

	void HideQuitGameMenu();

	UFUNCTION(BlueprintCallable)
	UEndGamePrompt* ShowEndGamePrompt();

	void HideEndGamePrompt();
};
