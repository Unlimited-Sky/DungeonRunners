// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DungeonRunnersCharacter.h"
#include "GlobalEventSystem.h"
#include "AbilitySystemComponent.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRUNNERS_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ADungeonRunnersCharacter*, float> ThreatMap;

	UPROPERTY()
	UGlobalEventSystem* GlobalEventSystem;

	void BeginPlay();
	void SortThreatMap();

	UFUNCTION()
	void OnEffectThreatHandler(float ThreatAmount, ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target);

	UFUNCTION()
	void OnCharacterSeenHandler(ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target);

	UFUNCTION()
	void OnEffectAppliedSelf(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION()
	void OnTauntCountChanged(const FGameplayTag Tag, int32 Count);

	UPROPERTY()
	TArray<ADungeonRunnersCharacter*> ThreatCharactersSorted;

	UPROPERTY()
	TArray<float> ThreatValuesSorted;

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners")
	void FindTopThreat();

protected:

	void OnPossess(APawn* InPawn);

public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsTaunted = false;

	UPROPERTY(BlueprintReadWrite)
	ADungeonRunnersCharacter* CurrentTarget;

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners")
	void OnTaunt(ADungeonRunnersCharacter* Taunter);

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners")
	TArray<ADungeonRunnersCharacter*> GetSortedCharacters() { return ThreatCharactersSorted; }

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners")
	TArray<float> GetSortedThreatValues() { return ThreatValuesSorted; }

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners")
	bool TryAddCharacterToThreatMap(ADungeonRunnersCharacter* Target);
};
