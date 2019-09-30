// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemyAttributeSet.h"

UBaseEnemyAttributeSet::UBaseEnemyAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = 3000.f;
	MaxHealth = 3000.f;
	HealthRegen = 1.f;

	Energy = 1000.f;
	MaxEnergy = 1000.f;
	EnergyRegen = 5.f;

	Movespeed = 600.f;

	DefenseModifier = 0.f;
	OffenseModifier = 0.f;

	HealingOutputModifier = 0.f;
	HealingReceivedModifier = 0.0f;
	
	ThreatModifier = 0.0f;

	BuffDurationModifier = 0.f;
	DebuffDurationModifier = 0.f;

	Haste = 1.f;
	CritChance = 0.20f;
	CritDamage = 1.f;
}


