// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GlobalEventSystem.h"
#include "DungeonRunnersGameState.generated.h"

class ADungeonRunnersCharacter;

/**
 * 
 */
UCLASS()
class DUNGEONRUNNERS_API ADungeonRunnersGameState : public AGameStateBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon Runners", meta = (AllowPrivateAccess = "true"))
	class UGlobalEventSystem* GlobalEventSystem;
public:

	UFUNCTION()
	void BeginPlay();

	UFUNCTION(BlueprintPure, Category = "Dungeon Runners", meta = (WorldContext = "WorldContextObject"))
	static class ADungeonRunnersGameState* GetDungeonRunnersGameState(const UObject* WorldContextObject);

	ADungeonRunnersGameState();

	UFUNCTION()
	class UGlobalEventSystem* GetGlobalEventSystem() const { return GlobalEventSystem; }
	
};
