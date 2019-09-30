// Fill out your copyright notice in the Description page of Project Settings.

#include "NinjaAttributeSet.h"

UNinjaAttributeSet::UNinjaAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = 3000.f;
	MaxHealth = 3000.f;
	HealthRegen = 1.f;

	Energy = 100.f;
	MaxEnergy = 100.f;
	EnergyRegen = 20.f;

	Movespeed = 600.f;

	DefenseModifier = 0.f;
	OffenseModifier = 0.f;

	HealingOutputModifier = 0.f;
	HealingReceivedModifier = 0.f;

	ThreatModifier = 0.f;

	BuffDurationModifier = 0.f;
	DebuffDurationModifier = 0.f;

	Haste = 1.f;
	CritChance = 0.30f;
	CritDamage = 1.f;
}




