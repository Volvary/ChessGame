#pragma once

#include "BoardLayers.generated.h"

USTRUCT(BlueprintType)
struct FBoardLayer
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FBoardRow : public FBoardLayer
{
	GENERATED_BODY()

public:
	//Using an array of Layers instead of Column for polymorphism when dealing with 3D or 4D boards
	TArray<FBoardLayer> Column;
};

USTRUCT(BlueprintType)
struct FBoardColumn : public FBoardLayer
{
	GENERATED_BODY()

	TArray<AActor*> Tiles;
};