// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DungeonRunnersCharacter.h"
#include "GlobalEventSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAbilityEventDelegate_OnEffectThreat, float, ThreatAmount, ADungeonRunnersCharacter*, Source, ADungeonRunnersCharacter*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAbilityEventDelegate_OnEffectDamage, float, DamageAmount, ADungeonRunnersCharacter*, Source, ADungeonRunnersCharacter*, Target, bool, Crit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAbilityEventDelegate_OnEffectHeal, float, HealAmount, ADungeonRunnersCharacter*, Source, ADungeonRunnersCharacter*, Target, bool, Crit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameEventDelegate_OnCharacterSeen, ADungeonRunnersCharacter*, Source, ADungeonRunnersCharacter*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameEventDelegate_OnCharacterKilled, ADungeonRunnersCharacter*, Source, ADungeonRunnersCharacter*, Target);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONRUNNERS_API UGlobalEventSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGlobalEventSystem();

public:	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Ability Event | On Effect Threat")
	FAbilityEventDelegate_OnEffectThreat OnEffectThreat;
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastEffectThreat(float ThreatAmount, ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Ability Event | On Effect Damage")
	FAbilityEventDelegate_OnEffectDamage OnEffectDamage;
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastEffectDamage(float DamageAmount, ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target, bool Crit);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Ability Event | On Effect Heal")
	FAbilityEventDelegate_OnEffectHeal OnEffectHeal;
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastEffectHeal(float HealAmount, ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target, bool Crit);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game Event | On Character Seen")
	FGameEventDelegate_OnCharacterSeen OnCharacterSeen;
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastCharacterSeen(ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game Event | On Character Killed")
	FGameEventDelegate_OnCharacterKilled OnCharacterKilled;
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastCharacterKilled(ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target);
};
