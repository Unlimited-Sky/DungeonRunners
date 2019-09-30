// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseDamageExecCalc.h"
#include "BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GlobalEventSystem.h"
#include "DungeonRunnersCharacter.h"
#include "DungeonRunnersGameState.h"
#include "GameplayeffectCalculation.h"

struct BaseDamageAttStruct
{
	//MaxHealth
	//Movespeed
	//BuffDurationModifier 
	//DebuffDurationModifier
	//Haste
	//HealingOutputModifier
	//HealingReceivedModifier

	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefenseModifier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(OffenseModifier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ThreatModifier);

	BaseDamageAttStruct()
	{
		//Last bool -> use initial snapshot value if true
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DefenseModifier, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, OffenseModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ThreatModifier, Source, false);
	}
};

BaseDamageAttStruct& DamageAtt()
{
	static BaseDamageAttStruct It;
	return It;
}

UBaseDamageExecCalc::UBaseDamageExecCalc(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RelevantAttributesToCapture.Add(DamageAtt().HealthDef);
	//InvalidScopedModifierAttributes.Add(DamageAtt().HealthDef);

	RelevantAttributesToCapture.Add(DamageAtt().DefenseModifierDef);
	RelevantAttributesToCapture.Add(DamageAtt().OffenseModifierDef);
	RelevantAttributesToCapture.Add(DamageAtt().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageAtt().CritDamageDef);
	RelevantAttributesToCapture.Add(DamageAtt().ThreatModifierDef);
}

void UBaseDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	//Set up some helpers
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent(); //We put AbilitySystemComponents into little helper variables. Not necessary, but it helps keeping us from typing so much.
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	ADungeonRunnersCharacter* SourceActor = SourceAbilitySystemComponent ? Cast<ADungeonRunnersCharacter>(SourceAbilitySystemComponent->AvatarActor) : nullptr; //If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing by trying to get the AvatarActor variable from
	ADungeonRunnersCharacter* TargetActor = TargetAbilitySystemComponent ? Cast<ADungeonRunnersCharacter>(TargetAbilitySystemComponent->AvatarActor) : nullptr; //a null pointer.

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags(); //Some more helper variables: Spec is the spec this execution originated from, and the Source/TargetTags are pointers to the tags granted to source/target actor, respectively.

	FAggregatorEvaluateParameters EvaluationParameters; //We use these tags to set up an FAggregatorEvaluateParameters struct, which we will need to get the values of our captured attributes later in this function.
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//Prepare our attributes
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAtt().HealthDef, EvaluationParameters, Health);

	float DefenseModifier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAtt().DefenseModifierDef, EvaluationParameters, DefenseModifier);

	float OffenseModifier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAtt().OffenseModifierDef, EvaluationParameters, OffenseModifier);

	float CritChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAtt().CritChanceDef, EvaluationParameters, CritChance);

	float CritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAtt().CritDamageDef, EvaluationParameters, CritDamage);

	float ThreatModifier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAtt().ThreatModifierDef, EvaluationParameters, ThreatModifier);

	//Start the damage formula
	bool Crit = false;
	//Calculate the damage from Min/Max value
	float MinDamage = Spec.GetSetByCallerMagnitude(FName(TEXT("MinValue")), true, 0.f);
	float MaxDamage = Spec.GetSetByCallerMagnitude(FName(TEXT("MaxValue")), true, 0.f);

	float FlatThreat = Spec.GetSetByCallerMagnitude(FName(TEXT("FlatThreat")), true, 0.f);

	float DamageDone = (FMath::FRand() * (MaxDamage - MinDamage)) + MinDamage;

	//Adjust with modifiers
	DamageDone *= (1.f + OffenseModifier);
	DamageDone *= (1.f - DefenseModifier);

	UGlobalEventSystem* EventSystem = ADungeonRunnersGameState::GetDungeonRunnersGameState(TargetAbilitySystemComponent->GetWorld())->GetGlobalEventSystem();
	
	//Did we even do any damage?
	if (DamageDone > 0.f)
	{
		//Check for a crit
		if (FMath::FRand() <= CritChance)
		{
			DamageDone *= 1.f + CritDamage;
			Crit = true;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Crit!"));

		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("DamageDone: %f"), Spec.StackCount * DamageDone));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageAtt().HealthProperty, EGameplayModOp::Additive, -DamageDone));
		EventSystem->BroadcastEffectDamage(DamageDone * Spec.StackCount, SourceActor, TargetActor, Crit);
		EventSystem->BroadcastEffectThreat(Spec.StackCount * (DamageDone + FlatThreat) * (1.f + ThreatModifier), SourceActor, TargetActor);
	}
	else if (FlatThreat > 0.f)
	{
		EventSystem->BroadcastEffectThreat(Spec.StackCount * FlatThreat * (1.f + ThreatModifier), SourceActor, TargetActor);
	}
}
