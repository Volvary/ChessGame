#pragma once

#include "PieceFamilyType.generated.h"

UENUM(BlueprintType)
enum class EPieceFamilyType : uint8 { 
	None,
	PlaceholderBlack, 
	PlaceholderWhite, 
	ClassicWhite, 
	ClassicBlack, 
};