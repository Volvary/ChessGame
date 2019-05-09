// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessHUD.h"

#include "GameClasses/Interfaces/PawnPromotionInterface.h"
#include "GameClasses/Interfaces/GameStatusInterface.h"
#include "GameClasses/Interfaces/TurnStatusInterface.h"
#include "GameClasses/Interfaces/SettingsMenu.h"
#include "GameClasses/Interfaces/QuitMenu.h"
#include "GameClasses/Interfaces/EndGamePrompt.h"

#include "ChessGame.h"

#include "GameClasses/InGameGM.h"

#include "Blueprint/UserWidget.h"

UUserWidget* AChessHUD::ShowInterface(TSubclassOf<UUserWidget> Widget, ESlateVisibility Visibility)
{
	if (Widget == nullptr)
	{
		UE_LOG(LogChess, Error, TEXT("ShowInterface was called on a nullptr reference. Aborted."));
		return nullptr;
	}

	UUserWidget** WidgetRef = InterfaceMap.Find(Widget);

	if (WidgetRef != nullptr)
	{
		(*WidgetRef)->SetVisibility(Visibility);

		return *WidgetRef;
	}
	else
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(PlayerOwner, Widget);
		NewWidget->AddToViewport(GetZOrder(Widget));

		InterfaceMap.Add(Widget, NewWidget);

		if (NewWidget != nullptr)
		{
			NewWidget->SetVisibility(Visibility);

			return NewWidget;
		}
	}

	return nullptr;
}

void AChessHUD::HideInterface(TSubclassOf<UUserWidget> Widget)
{
	UUserWidget** WidgetRef = InterfaceMap.Find(Widget);

	if (WidgetRef != nullptr)
	{
		(*WidgetRef)->SetVisibility(ESlateVisibility::Hidden);
	}
}

int AChessHUD::GetZOrder(TSubclassOf<UUserWidget> Widget)
{
	int Out = -1;
	
	int* ZOrder = InterfaceZOrder.Find(Widget);

	if (ZOrder != nullptr)
	{
		Out = *ZOrder;
	}
	else
	{
		UE_LOG(LogChess, Warning, TEXT("GetZOrder could not find the class requested. Using -1."));
	}

	return Out;
}

void AChessHUD::ShowPromotionInterface()
{
	UPawnPromotionInterface* PPInterface = Cast<UPawnPromotionInterface> (ShowInterface(PawnPromotionInterfaceClass));

	PPInterface->SetGameMode(Cast<AInGameGM>(GetWorld()->GetAuthGameMode()));

}

void AChessHUD::HidePromotionInterface()
{
	HideInterface(PawnPromotionInterfaceClass);
}

UTurnStatusInterface* AChessHUD::ShowTurnStatusInterface()
{
	UTurnStatusInterface* Interface = Cast<UTurnStatusInterface>(ShowInterface(TurnStatusInterfaceClass));

	return Interface;
}

void AChessHUD::HideTurnStatusInterface()
{
	HideInterface(TurnStatusInterfaceClass);
}

USettingsMenu* AChessHUD::ShowSettingsMenu()
{
	USettingsMenu* Interface = Cast<USettingsMenu>(ShowInterface(SettingsMenuClass));

	return Interface;
}

void AChessHUD::HideSettingsMenu()
{
	HideInterface(SettingsMenuClass);
}

UGameStatusInterface* AChessHUD::ShowGameStatusInterface()
{
	UGameStatusInterface* Interface = Cast<UGameStatusInterface>(ShowInterface(GameStatusInterfaceClass));

	Interface->SetHUD(this);

	return Interface;
}

void AChessHUD::HideGameStatusInterface()
{
	HideInterface(GameStatusInterfaceClass);
}

void AChessHUD::ShowQuitGameMenu()
{
	ShowInterface(QuitMenuClass);
}

void AChessHUD::HideQuitGameMenu()
{
	HideInterface(QuitMenuClass);
}

UEndGamePrompt* AChessHUD::ShowEndGamePrompt()
{
	UEndGamePrompt* Prompt = Cast<UEndGamePrompt>(ShowInterface(EndGamePromptClass));

	return Prompt;
}

void AChessHUD::HideEndGamePrompt()
{
	HideInterface(EndGamePromptClass);
}
