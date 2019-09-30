// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalEventSystem.h"
#include "UnrealNetwork.h"

UGlobalEventSystem::UGlobalEventSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGlobalEventSystem::BroadcastEffectDamage_Implementation(float DamageAmount, ADungeonRunnersCharacter * Source, ADungeonRunnersCharacter * Target, bool Crit)
{
	OnEffectDamage.Broadcast(DamageAmount, Source, Target, Crit);
}

void UGlobalEventSystem::BroadcastEffectHeal_Implementation(float HealAmount, ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target, bool Crit)
{
	OnEffectHeal.Broadcast(HealAmount, Source, Target, Crit);
}

void UGlobalEventSystem::BroadcastEffectThreat_Implementation(float ThreatAmount, ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target)
{
	OnEffectThreat.Broadcast(ThreatAmount, Source, Target);
}

void UGlobalEventSystem::BroadcastCharacterSeen_Implementation(ADungeonRunnersCharacter * Source, ADungeonRunnersCharacter * Target)
{
	OnCharacterSeen.Broadcast(Source, Target);
}

void UGlobalEventSystem::BroadcastCharacterKilled_Implementation(ADungeonRunnersCharacter * Source, ADungeonRunnersCharacter * Target)
{
	OnCharacterKilled.Broadcast(Source, Target);
}