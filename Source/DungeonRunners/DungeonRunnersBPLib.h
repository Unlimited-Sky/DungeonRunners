// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "DungeonRunnersBPLib.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRUNNERS_API UDungeonRunnersBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
	static FActiveGameplayEffectHandle ApplyDungeonRunnersStandardEffectSpecToTarget(
		TSubclassOf<UGameplayEffect> EffectClass,
		UAbilitySystemComponent* Source,
		UAbilitySystemComponent* Target,
		float MinValue,
		float MaxValue,
		float FlatThreat = 0.f,
		float Level = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
		static void SetBaseAttributes(
			UAbilitySystemComponent* Target,
			float Health = 2000.f,
			float MaxHealth = 2000.f,
			float Movespeed = 600.f,
			float DefenseModifier = 0.f,
			float OffenseModifier = 0.f,
			float HealingOutputModifier = 0.f,
			float HealingReceivedModifier = 0.f,
			float ThreatModifier = 0.f,
			float BuffDurationModifier = 0.f,
			float DebuffDurationModifier = 0.f,
			float Haste = 1.0f,
			float CritChance = 0.25f,
			float CritDamage = 1.f);
	
	//Returns true if actors have the same team
	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
	static bool CheckActorsSameTeam(AActor* Actor1, AActor* Actor2);

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
	static void ApplyEnergyCost(UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> EffectClass, float EnergyCost);

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
	static float GetCooldownTimeRemaining(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle AbilitySpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
	static void GetCooldownTimeRemainingAndDuration(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle AbilitySpecHandle, float & TimeRemaining, float & CooldownDuration);

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
	static void EquipAbilityToSlot(ADungeonRunnersCharacter* Target, TSubclassOf<class UGameplayAbility> Ability, int32 Slot);

	UFUNCTION(BlueprintCallable, Category = "Dungeon Runners Library")
	static void RemoveAbilityFromSlot(ADungeonRunnersCharacter* Target, TSubclassOf<class UGameplayAbility> Ability, int32 Slot);
};