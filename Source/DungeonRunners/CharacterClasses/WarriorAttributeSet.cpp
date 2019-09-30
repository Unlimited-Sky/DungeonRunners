// Fill out your copyright notice in the Description page of Project Settings.

#include "WarriorAttributeSet.h"

UWarriorAttributeSet::UWarriorAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = 3500.f;
	MaxHealth = 3500.f;
	HealthRegen = 1.f;

	Energy = 100.f;
	MaxEnergy = 100.f;
	EnergyRegen = 20.f;

	Movespeed = 600.f;

	DefenseModifier = 0.4f;
	OffenseModifier = 0.f;

	HealingOutputModifier = 0.f;
	HealingReceivedModifier = 0.2f;

	ThreatModifier = 0.5f;

	BuffDurationModifier = 0.f;
	DebuffDurationModifier = 0.f;

	Haste = 1.f;
	CritChance = 0.25f;
	CritDamage = 1.f;
}


