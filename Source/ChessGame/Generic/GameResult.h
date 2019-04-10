#pragma once

#include "GameResult.generated.h"

UENUM(BlueprintType)
enum class EGameResult : uint8
{
	Cancelled,
	Draw,
	Stalemate,
	Checkmate,
	Forfeit,
};