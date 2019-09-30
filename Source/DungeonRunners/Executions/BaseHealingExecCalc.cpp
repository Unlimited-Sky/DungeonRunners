// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseHealingExecCalc.h"
#include "BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GlobalEventSystem.h"
#include "DungeonRunnersCharacter.h"
#include "DungeonRunnersGameState.h"

struct BaseHealingAttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealingOutputModifier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealingReceivedModifier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ThreatModifier);

	BaseHealingAttStruct()
	{
		//Last bool -> use initial snapshot value if true
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, HealingOutputModifier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, HealingReceivedModifier, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ThreatModifier, Source, false);
	}
};

BaseHealingAttStruct& HealingAtt()
{
	static BaseHealingAttStruct It;
	return It;
}

UBaseHealingExecCalc::UBaseHealingExecCalc(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RelevantAttributesToCapture.Add(HealingAtt().HealthDef);
	//InvalidScopedModifierAttributes.Add(HealingAtt().HealthDef);

	RelevantAttributesToCapture.Add(HealingAtt().MaxHealthDef);
	RelevantAttributesToCapture.Add(HealingAtt().HealingOutputModifierDef);
	RelevantAttributesToCapture.Add(HealingAtt().HealingReceivedModifierDef);
	RelevantAttributesToCapture.Add(HealingAtt().CritChanceDef);
	RelevantAttributesToCapture.Add(HealingAtt().CritDamageDef);
	RelevantAttributesToCapture.Add(HealingAtt().ThreatModifierDef);
}

void UBaseHealingExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
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

	UGlobalEventSystem* EventSystem = ADungeonRunnersGameState::GetDungeonRunnersGameState(TargetAbilitySystemComponent->GetWorld())->GetGlobalEventSystem();

	//Prepare our attributes
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingAtt().HealthDef, EvaluationParameters, Health);

	float MaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingAtt().MaxHealthDef, EvaluationParameters, MaxHealth);

	float HealingOutputModifier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingAtt().HealingOutputModifierDef, EvaluationParameters, HealingOutputModifier);

	float HealingReceivedModifier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingAtt().HealingReceivedModifierDef, EvaluationParameters, HealingReceivedModifier);

	float CritChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingAtt().CritChanceDef, EvaluationParameters, CritChance);

	float CritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingAtt().CritDamageDef, EvaluationParameters, CritDamage);

	float ThreatModifier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingAtt().ThreatModifierDef, EvaluationParameters, ThreatModifier);

	//Start the damage formula
	bool Crit = false;
	//Calculate the damage from Min/Max value
	float MinHealing = Spec.GetSetByCallerMagnitude(FName(TEXT("MinValue")), true, 0.f);
	float MaxHealing = Spec.GetSetByCallerMagnitude(FName(TEXT("MaxValue")), true, 0.f);

	float FlatThreat = Spec.GetSetByCallerMagnitude(FName(TEXT("FlatThreat")), true, 0.f);

	float HealingDone = (FMath::FRand() * (MaxHealing - MinHealing)) + MinHealing;

	//Adjust with modifiers
	HealingDone *= (1.f + HealingOutputModifier);
	HealingDone *= (1.f + HealingReceivedModifier);

	//Check for a crit
	if (FMath::FRand() <= CritChance)
	{
		HealingDone *= 1.f + CritDamage;
		Crit = true;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Crit!"));
	}

	HealingDone = FMath::Min(HealingDone, (MaxHealth - Health));

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Healing!"));

	//Did we even do any healing?
	if (HealingDone > 0.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HealingDone: %f"), HealingDone));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealingAtt().HealthProperty, EGameplayModOp::Additive, HealingDone));
		EventSystem->BroadcastEffectHeal(HealingDone, SourceActor, TargetActor, Crit);
		EventSystem->BroadcastEffectThreat((HealingDone + FlatThreat) * (1.f + ThreatModifier), SourceActor, TargetActor);
	}
	else if (FlatThreat > 0.f)
	{
		EventSystem->BroadcastEffectThreat(FlatThreat * (1.f + ThreatModifier), SourceActor, TargetActor);
	}
}
