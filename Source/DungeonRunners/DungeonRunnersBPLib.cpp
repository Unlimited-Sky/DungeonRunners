// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonRunnersBPLib.h"
#include "DungeonRunnersCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

FActiveGameplayEffectHandle UDungeonRunnersBPLib::ApplyDungeonRunnersStandardEffectSpecToTarget(TSubclassOf<UGameplayEffect> EffectClass, UAbilitySystemComponent * Source, UAbilitySystemComponent * Target, float MinValue, float MaxValue, float FlatThreat, float Level)
{
	if (Source)
	{
		FGameplayEffectContextHandle ContextHandle = Source->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		Spec->SetSetByCallerMagnitude(FName("MinValue"), MinValue);
		Spec->SetSetByCallerMagnitude(FName("MaxValue"), MaxValue);

		if (FlatThreat != 0.f)
		{
			Spec->SetSetByCallerMagnitude(FName("FlatThreat"), FlatThreat);
		}

		return Target->ApplyGameplayEffectSpecToSelf(*Spec);
	}
	return FActiveGameplayEffectHandle();
}

void UDungeonRunnersBPLib::SetBaseAttributes(UAbilitySystemComponent* Target, float Health, float MaxHealth, float Movespeed, float DefenseModifier, float OffenseModifier, float HealingOutputModifier, float HealingReceivedModifier, float ThreatModifier, float BuffDurationModifier, float DebuffDurationModifier, float Haste, float CritChance, float CritDamage)
{
	//FGameplayEffectSpecHandle SpecHandle = Target->MakeOutgoingSpec(UGameplayEffect::StaticClass(), 0.f, Target->MakeEffectContext());
	//FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	//Spec->SetSetByCallerMagnitude(FName("Health"), Health);
	//Spec->SetSetByCallerMagnitude(FName("MaxHealth"), MaxHealth);
	//Spec->SetSetByCallerMagnitude(FName("Movespeed"), Movespeed);
	//Spec->SetSetByCallerMagnitude(FName("DefenseModifier"), DefenseModifier);
	//Spec->SetSetByCallerMagnitude(FName("OffenseModifier"), OffenseModifier);
	//Spec->SetSetByCallerMagnitude(FName("HealingOutputModifier"), HealingOutputModifier);
	//Spec->SetSetByCallerMagnitude(FName("HealingReceivedModifier"), HealingReceivedModifier);
	//Spec->SetSetByCallerMagnitude(FName("ThreatModifier"), ThreatModifier);
	//Spec->SetSetByCallerMagnitude(FName("BuffDurationModifier"), BuffDurationModifier);
	//Spec->SetSetByCallerMagnitude(FName("DebuffDurationModifier"), DebuffDurationModifier);
	//Spec->SetSetByCallerMagnitude(FName("Haste"), Haste);
	//Spec->SetSetByCallerMagnitude(FName("CritChance"), CritChance);
	//Spec->SetSetByCallerMagnitude(FName("CritDamage"), CritDamage);

	TArray<FGameplayAttribute> Attributes;

	Target->GetAllAttributes(Attributes);

	//Attributes[0].GetUProperty(

	for (int i = 0; i < Attributes.Num(); i++)
	{
		Target->ApplyModToAttribute(Attributes[i], EGameplayModOp::Override, Health);
	}

	////Target->ApplyModToAttribute()

	//FGameplayAttribute HealthAttribute;
	////HealthAttribute.AttributeName = "BaseAttributeSet.Health";
	////HealthAttribute.
	////HealthAttribute.GetGameplayAttributeData()->SetBaseValue(Health);
	////HealthAttribute.GetGameplayAttributeData()->SetCurrentValue(Health);
	////HealthAttribut

	//HealthAttribute.

	////FGameplayAttribute MaxHealthAttribute;
	////MaxHealthAttribute.AttributeName = "BaseAttributeSet.MaxHealth";
	////MaxHealthAttribute.Set

	////Spec->AddModifiedAttribute(HealthAttribute);

	//Target->ApplyGameplayEffectSpecToSelf(*Spec);
}

bool UDungeonRunnersBPLib::CheckActorsSameTeam(AActor* Actor1, AActor* Actor2)
{
	ADungeonRunnersCharacter* Character1 = Actor1 ? Cast<ADungeonRunnersCharacter>(Actor1) : nullptr;
	ADungeonRunnersCharacter* Character2 = Actor2 ? Cast<ADungeonRunnersCharacter>(Actor2) : nullptr;

	if (Character1 != NULL && Character2 != NULL)
	{
		return Character1->TeamID == Character2->TeamID;
	}

	return false;
}

void UDungeonRunnersBPLib::ApplyEnergyCost(UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> EffectClass, float EnergyCost)
{
	if (Target)
	{
		FGameplayEffectContextHandle ContextHandle = Target->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = Target->MakeOutgoingSpec(EffectClass, 0.f, ContextHandle);
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		Spec->SetSetByCallerMagnitude(FName("EnergyCost"), -EnergyCost);
		Target->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

float UDungeonRunnersBPLib::GetCooldownTimeRemaining(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	if (AbilitySpecHandle.IsValid())
		return AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability->GetCooldownTimeRemaining(AbilitySystemComponent->AbilityActorInfo.Get());
	else
		return 0.0f;
}

void UDungeonRunnersBPLib::GetCooldownTimeRemainingAndDuration(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle AbilitySpecHandle, float & TimeRemaining, float & CooldownDuration)
{
	if (AbilitySystemComponent && AbilitySpecHandle.IsValid())
	{
		AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability->GetCooldownTimeRemainingAndDuration(AbilitySpecHandle, AbilitySystemComponent->AbilityActorInfo.Get(), TimeRemaining, CooldownDuration);
	}
}

void UDungeonRunnersBPLib::RemoveAbilityFromSlot(ADungeonRunnersCharacter* Target, TSubclassOf<class UGameplayAbility> Ability, int32 Slot)
{
	if (Target)
	{
		UAbilitySystemComponent* AbilitySystem = Target->AbilitySystem;

		if (Target->AbilityHandles[Slot].IsValid())
		{
			FGameplayAbilitySpecHandle HandleToRemove = Target->AbilityHandles[Slot];
			AbilitySystem->ClearAbility(HandleToRemove);
			Target->AbilityHandles[Slot] = FGameplayAbilitySpecHandle();
			Target->Abilities[Slot] = NULL;
		}
	}
}

void UDungeonRunnersBPLib::EquipAbilityToSlot(ADungeonRunnersCharacter* Target, TSubclassOf<class UGameplayAbility> Ability, int32 Slot)
{
	if (Target)
	{
		UAbilitySystemComponent* AbilitySystem = Target->AbilitySystem;

		if (IsValid(Ability))
		{

			if (Target->AbilityHandles[Slot].IsValid())
			{
				FGameplayAbilitySpecHandle HandleToRemove = Target->AbilityHandles[Slot];
				AbilitySystem->ClearAbility(HandleToRemove);
			}

			FGameplayAbilitySpec SpecToAdd = FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, Slot);
			FGameplayAbilitySpecHandle Handle = AbilitySystem->GiveAbility(SpecToAdd);
			Target->AbilityHandles[Slot] = Handle;
			Target->Abilities[Slot] = Ability;
		}
	}
}
