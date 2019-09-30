// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "BaseDamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRUNNERS_API UBaseDamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()

		virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	
};
