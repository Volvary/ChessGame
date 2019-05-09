// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SoundEventReferences.h"

#include "SoundManager.generated.h"


class UAkAudioEvent;

UCLASS()
class CHESSGAME_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<SoundEvents, UAkAudioEvent*> EnumToSoundEvent;


public:	
	// Sets default values for this actor's properties
	ASoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
