// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAttributeSet.h"
#include "UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "DungeonRunnersCharacter.h"
#include "GlobalEventSystem.h"
#include "DungeonRunnersGameState.h"
#include "DungeonRunnersCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
//#include "GameplayTagContainer.h"
//#include "GameplayTagsModule.h"

UBaseAttributeSet::UBaseAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = 500.f;
	MaxHealth = 500.f;
	HealthRegen = 15.f;

	Energy = 500.f;
	MaxEnergy = 1000.f;
	EnergyRegen = 5.f;

	Movespeed = 600.f;

	DefenseModifier = 0.f;
	OffenseModifier = 0.f;

	HealingOutputModifier = 0.f;
	HealingReceivedModifier = 0.f;

	ThreatModifier = 0.f;

	BuffDurationModifier = 0.f;
	DebuffDurationModifier = 0.f;

	Haste = 1.f;
	CritChance = 0.25f;
	CritDamage = 1.f;
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, EnergyRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Movespeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, DefenseModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, OffenseModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealingOutputModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealingReceivedModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ThreatModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BuffDurationModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, DebuffDurationModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Haste, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::OnRep_Health()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health)
}

void UBaseAttributeSet::OnRep_MaxHealth()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth)
}

void UBaseAttributeSet::OnRep_HealthRegen()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealthRegen)
}

void UBaseAttributeSet::OnRep_Energy()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Energy)
}

void UBaseAttributeSet::OnRep_MaxEnergy()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxEnergy)
}

void UBaseAttributeSet::OnRep_EnergyRegen()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, EnergyRegen)
}

void UBaseAttributeSet::OnRep_Movespeed()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Movespeed)
}

void UBaseAttributeSet::OnRep_DefenseModifier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, DefenseModifier)
}

void UBaseAttributeSet::OnRep_OffenseModifier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, OffenseModifier)
}

void UBaseAttributeSet::OnRep_HealingOutputModifier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealingOutputModifier)
}

void UBaseAttributeSet::OnRep_BuffDurationModifier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BuffDurationModifier)
}

void UBaseAttributeSet::OnRep_DebuffDurationModifier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, DebuffDurationModifier)
}

void UBaseAttributeSet::OnRep_HealingReceivedModifier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealingReceivedModifier)
}

void UBaseAttributeSet::OnRep_ThreatModifier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ThreatModifier)
}

void UBaseAttributeSet::OnRep_Haste()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Haste)
}

void UBaseAttributeSet::OnRep_CritChance()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CritChance)
}

void UBaseAttributeSet::OnRep_CritDamage()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CritDamage)
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	// Get the Target actor
	AActor* TargetActor = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
	}

	//Handle Health Changes
	if (HealthAttribute() == Data.EvaluatedData.Attribute)
	{
		// Clamp health
		Health = FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue());
		if (Health.GetCurrentValue() <= 0)
		{
			if (ADungeonRunnersCharacter* Character = Cast<ADungeonRunnersCharacter>(TargetActor))
			{
				UGlobalEventSystem* EventSystem = ADungeonRunnersGameState::GetDungeonRunnersGameState(Character->GetWorld())->GetGlobalEventSystem();
				EventSystem->OnCharacterKilled.Broadcast(Cast<ADungeonRunnersCharacter>(SourceActor), Character);

				Character->Destroy();
			}
		}
	}

	//Handle Energy Changes
	if (EnergyAttribute() == Data.EvaluatedData.Attribute)
	{
		Energy = FMath::Clamp(Energy.GetCurrentValue(), 0.f, MaxEnergy.GetCurrentValue());
	}
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) 
{ 
	if (MovespeedAttribute() == Attribute)
	{
		ADungeonRunnersCharacter* Owner = Cast<ADungeonRunnersCharacter>(GetOwningActor());
		Owner->GetCharacterMovement()->MaxWalkSpeed = NewValue;
	}
}


FGameplayAttribute UBaseAttributeSet::HealthAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, Health));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UBaseAttributeSet::EnergyAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, Energy));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UBaseAttributeSet::MaxHealthAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, MaxHealth));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UBaseAttributeSet::MaxEnergyAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, MaxEnergy));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UBaseAttributeSet::MovespeedAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, Movespeed));
	return FGameplayAttribute(Property);
}