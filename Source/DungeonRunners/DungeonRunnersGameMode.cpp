// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DungeonRunnersGameMode.h"
#include "DungeonRunnersCharacter.h"
#include "DungeonRunnersGameState.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"

ADungeonRunnersGameMode::ADungeonRunnersGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = ADungeonRunnersGameState::StaticClass();

	static ConstructorHelpers::FClassFinder<AHUD> CustomHUDClass(TEXT("/Game/Blueprints/HUD/DungeonRunnersHUD"));
	if (CustomHUDClass.Class != NULL)
	{
		HUDClass = CustomHUDClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> CustomPlayerControllerClass(TEXT("/Game/Blueprints/DungeonRunnersPlayerController"));
	if (CustomPlayerControllerClass.Class != NULL)
	{
		PlayerControllerClass = CustomPlayerControllerClass.Class;
	}*/
}
