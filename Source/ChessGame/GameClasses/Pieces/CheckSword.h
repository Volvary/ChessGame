// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckSword.generated.h"

/*
*	Actor used to symbolize that a player's royal piece is in check. Instantiated at the start of the game and moved / shown on Check.
*	Smashes down on the royal piece on Checkmate.
*/

UCLASS()
class CHESSGAME_API ACheckSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ShowSword();

	UFUNCTION(BlueprintNativeEvent)
	void RunCheckmateAnimation();

};
