// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CollisionTargetActorCollider.h"
#include "CollisionTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRUNNERS_API ACollisionTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	AActor* CollisionTester;
	
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	
	/** Radius of target acquisition around the ability's start location. */
	/** Another Actor to be used for collision checking. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Collision Actor" )
	TSubclassOf<ACollisionTargetActorCollider> CollisionActor;

protected:
	virtual TArray<TWeakObjectPtr<AActor>> PerformOverlap(const FVector& Origin);

	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const;

};
