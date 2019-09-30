// Fill out your copyright notice in the Description page of Project Settings.

#include "MageAttributeSet.h"

UMageAttributeSet::UMageAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = 2000.f;
	MaxHealth = 2000.f;
	HealthRegen = 1.f;

	Energy = 1000.f;
	MaxEnergy = 1000.f;
	EnergyRegen = 40.f;

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

