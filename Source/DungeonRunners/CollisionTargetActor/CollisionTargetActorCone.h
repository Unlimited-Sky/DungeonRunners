// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Components/SphereComponent.h"
#include "CollisionTargetActorCone.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRUNNERS_API ACollisionTargetActorCone : public AGameplayAbilityTargetActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	class USphereComponent* Sphere;

	UPROPERTY(Replicated)
	class UDecalComponent* DecalComponent;

	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Cone Collision")
	class UMaterial* DecalMaterial;

	UPROPERTY(Replicated)
	class UMaterialInstanceDynamic* DecalMaterialDynamic;

	void Init();

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	/** Length of target acquisition around the ability's start location. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Cone Collision", Replicated)
	float Length;

	/** Angle of the cone collision check in Degrees. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Cone Collision", Replicated)
	float AngleInDegrees;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Cone Collision", Replicated)
	bool IncludeSelf;

	UFUNCTION(NetMulticast, Reliable)
	void Client_Init();

protected:
	virtual TArray<TWeakObjectPtr<AActor>> PerformTrace(const FVector& Origin);

	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const;

};