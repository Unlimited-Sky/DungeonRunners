// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

/**
*
*/
UCLASS(BlueprintType)
class DUNGEONRUNNERS_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Base Attributes")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "Base Attributes")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "Base Attributes")
		FGameplayAttributeData HealthRegen;
	UFUNCTION()
		void OnRep_HealthRegen();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Base Attributes")
		FGameplayAttributeData Energy;
	UFUNCTION()
		void OnRep_Energy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "Base Attributes")
		FGameplayAttributeData MaxEnergy;
	UFUNCTION()
		void OnRep_MaxEnergy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "Base Attributes")
		FGameplayAttributeData EnergyRegen;
	UFUNCTION()
		void OnRep_EnergyRegen();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Movespeed, Category = "Base Attributes")
	FGameplayAttributeData Movespeed;
	UFUNCTION()
	void OnRep_Movespeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_DefenseModifier, Category = "Base Attributes")
	FGameplayAttributeData DefenseModifier;
	UFUNCTION()
	void OnRep_DefenseModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_OffenseModifier, Category = "Base Attributes")
	FGameplayAttributeData OffenseModifier;
	UFUNCTION()
	void OnRep_OffenseModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HealingOutputModifier, Category = "Base Attributes")
	FGameplayAttributeData HealingOutputModifier;
	UFUNCTION()
	void OnRep_HealingOutputModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HealingReceivedModifier, Category = "Base Attributes")
	FGameplayAttributeData HealingReceivedModifier;
	UFUNCTION()
	void OnRep_HealingReceivedModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_BuffDurationModifier, Category = "Base Attributes")
	FGameplayAttributeData BuffDurationModifier;
	UFUNCTION()
	void OnRep_BuffDurationModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_DebuffDurationModifier, Category = "Base Attributes")
	FGameplayAttributeData DebuffDurationModifier;
	UFUNCTION()
	void OnRep_DebuffDurationModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ThreatModifier, Category = "Base Attributes")
	FGameplayAttributeData ThreatModifier;
	UFUNCTION()
	void OnRep_ThreatModifier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Haste, Category = "Base Attributes")
	FGameplayAttributeData Haste;
	UFUNCTION()
	void OnRep_Haste();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CritChance, Category = "Base Attributes")
	FGameplayAttributeData CritChance;
	UFUNCTION()
	void OnRep_CritChance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CritDamage, Category = "Base Attributes")
	FGameplayAttributeData CritDamage;
	UFUNCTION()
	void OnRep_CritDamage();

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	void PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue);

	static FGameplayAttribute HealthAttribute();
	static FGameplayAttribute MaxHealthAttribute();

	static FGameplayAttribute EnergyAttribute();
	static FGameplayAttribute MaxEnergyAttribute();

	static FGameplayAttribute MovespeedAttribute();
};