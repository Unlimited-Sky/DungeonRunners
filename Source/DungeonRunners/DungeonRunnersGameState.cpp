// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonRunnersGameState.h"
#include "UnrealNetwork.h"

ADungeonRunnersGameState::ADungeonRunnersGameState()
{
	GlobalEventSystem = CreateDefaultSubobject<UGlobalEventSystem>(TEXT("Global Event System"));
}

void ADungeonRunnersGameState::BeginPlay()
{
	Super::BeginPlay();
}


ADungeonRunnersGameState* ADungeonRunnersGameState::GetDungeonRunnersGameState(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	return World ? Cast<ADungeonRunnersGameState>(World->GetGameState()) : nullptr;
}