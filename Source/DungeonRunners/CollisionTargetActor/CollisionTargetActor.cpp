// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionTargetActor.h"
#include "DungeonRunnersCharacter.h"
#include "Abilities/GameplayAbility.h"

ACollisionTargetActor::ACollisionTargetActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	ShouldProduceTargetDataOnServer = true;
	bDestroyOnConfirmation = true;
}

void ACollisionTargetActor::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();

	CollisionTester = GetWorld()->SpawnActor<AActor>(CollisionActor.Get(), SourceActor->GetTransform());
	CollisionTester->AttachToActor(SourceActor, FAttachmentTransformRules::SnapToTargetIncludingScale);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, CollisionTester->GetActorLocation().ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FString("Start Targeting"));
}

void ACollisionTargetActor::ConfirmTargetingAndContinue()
{
	//check(ShouldProduceTargetData());
	if (SourceActor)
	{
		FVector Origin = StartLocation.GetTargetingTransform().GetLocation();
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformOverlap(Origin), Origin);
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

FGameplayAbilityTargetDataHandle ACollisionTargetActor::MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const
{
	if (OwningAbility)
	{
		/** Use the source location instead of the literal origin */
		return StartLocation.MakeTargetDataHandleFromActors(Actors, false);
	}

	return FGameplayAbilityTargetDataHandle();
}

TArray<TWeakObjectPtr<AActor>> ACollisionTargetActor::PerformOverlap(const FVector& Origin)
{
	TArray<AActor*> Overlaps;

	//AActor* CollisionTester = GetWorld()->SpawnActor<AActor>(CollisionActor.Get(), SourceActor->GetTransform());
	CollisionTester->GetOverlappingActors(Overlaps, ADungeonRunnersCharacter::StaticClass());
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(Overlaps.Num()));

	TArray<TWeakObjectPtr<AActor>> HitActors;

	for (int32 i = 0; i < Overlaps.Num(); ++i)
	{
		APawn* PawnActor = Cast<APawn>(Overlaps[i]);
		if (PawnActor && !HitActors.Contains(PawnActor) && Filter.FilterPassesForActor(PawnActor))
		{
			HitActors.Add(PawnActor);
		}
	}

	CollisionTester->Destroy();

	return HitActors;
}

