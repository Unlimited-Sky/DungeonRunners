// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
*
*/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECharacterClass : uint8
{
	CC_None 		UMETA(DisplayName = "None"),
	CC_Monk			UMETA(DisplayName = "Monk"),
	CC_Illusionist	UMETA(DisplayName = "Illusionist"),
	CC_Warrior		UMETA(DisplayName = "Warrior"),
	CC_Gunner		UMETA(DisplayName = "Gunner"),
	CC_Ranger		UMETA(DisplayName = "Ranger"),
	CC_Templar		UMETA(DisplayName = "Templar"),
	CC_Spellsword	UMETA(DisplayName = "Spellsword"),
	CC_Magus		UMETA(DisplayName = "Magus"),
	CC_Duelist		UMETA(DisplayName = "Duelist"),
	CC_Necromancer	UMETA(DisplayName = "Necromancer"),
	CC_Ninja		UMETA(DisplayName = "Ninja")
};
