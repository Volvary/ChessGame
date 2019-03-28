#pragma once

#include "BoardActionInformations.generated.h"

UENUM(BlueprintType)
enum class EBoardTileState : uint8 { None, Clicked, ClickedWaiting, ValidMove, ValidMoveWaiting, Capture, CaptureWaiting, Failed };