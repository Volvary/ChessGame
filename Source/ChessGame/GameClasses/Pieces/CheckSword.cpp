// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckSword.h"

// Sets default values
ACheckSword::ACheckSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACheckSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckSword::ShowSword()
{

}

void ACheckSword::RunCheckmateAnimation_Implementation()
{

}

