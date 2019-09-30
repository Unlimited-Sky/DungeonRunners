// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionTargetActorCone.h"
#include "DungeonRunnersCharacter.h"
#include "Abilities/GameplayAbility.h"
#include "UnrealNetwork.h"

ACollisionTargetActorCone::ACollisionTargetActorCone(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	ShouldProduceTargetDataOnServer = true;
	bDestroyOnConfirmation = true;
	SetReplicates(true);
}

void ACollisionTargetActorCone::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollisionTargetActorCone, DecalMaterial);
	DOREPLIFETIME(ACollisionTargetActorCone, Length);
	DOREPLIFETIME(ACollisionTargetActorCone, AngleInDegrees);
	DOREPLIFETIME(ACollisionTargetActorCone, DecalComponent);
	DOREPLIFETIME(ACollisionTargetActorCone, DecalMaterialDynamic);
}

void ACollisionTargetActorCone::Init()
{
	Sphere = NewObject<USphereComponent>(this);
	Sphere->InitSphereRadius(Length);
	Sphere->BodyInstance.SetCollisionProfileName(TEXT("Custom"));
	Sphere->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->BodyInstance.SetObjectType(ECC_WorldDynamic);
	Sphere->BodyInstance.SetResponseToChannels(ECR_Ignore);
	Sphere->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->RegisterComponent();
	RootComponent = Sphere;

	DecalComponent = NewObject<UDecalComponent>(this);
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->DecalSize = FVector(Length, Length, Length);
	DecalComponent->SetVisibility(true);
	DecalComponent->RegisterComponent();
	DecalComponent->AddLocalRotation(FRotator(-90.f, 0.f, 0.f));

	DecalMaterialDynamic = UMaterialInstanceDynamic::Create(DecalMaterial, this);
	DecalMaterialDynamic->SetScalarParameterValue(FName("AngleInRadians"), FMath::DegreesToRadians(AngleInDegrees));
	DecalComponent->SetMaterial(0, DecalMaterialDynamic);
}

void ACollisionTargetActorCone::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();

	if (SourceActor)
	{
		if (HasAuthority())
		{
			Init();
			Client_Init();
		}
		AttachToActor(SourceActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FString("NO SOURCE ACTOR!"));
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, CollisionTester->GetActorLocation().ToString());
}

void ACollisionTargetActorCone::ConfirmTargetingAndContinue()
{
	//check(ShouldProduceTargetData());
	if (SourceActor)
	{
		FVector Origin = StartLocation.GetTargetingTransform().GetLocation();
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformTrace(Origin), Origin);
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

FGameplayAbilityTargetDataHandle ACollisionTargetActorCone::MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const
{
	if (OwningAbility)
	{
		/** Use the source location instead of the literal origin */
		return StartLocation.MakeTargetDataHandleFromActors(Actors, false);
	}

	return FGameplayAbilityTargetDataHandle();
}

TArray<TWeakObjectPtr<AActor>> ACollisionTargetActorCone::PerformTrace(const FVector& Origin)
{
	//Logic:
	//1. Get all objects colliding with our first circle
	//2. Of those objects, get the ones who's positions are valid with vector magic
	//3. For any objects we want to prune, do a line trace across LEFT and RIGHT and save them (NOT IMPLEMENTED YET?)
	//4. We have our cone collision

	TArray<AActor*> InRange;
	TArray<TWeakObjectPtr<AActor>> HitActors;

	Sphere->GetOverlappingActors(InRange, ADungeonRunnersCharacter::StaticClass());

	if (IncludeSelf)
	{
		HitActors.Add(SourceActor);
	}

	for (int i = 0; i < InRange.Num(); i++)
	{
		FVector toTarget = (InRange[i]->GetActorLocation() - GetActorLocation());
		toTarget.Normalize();

		float dot = FVector::DotProduct(GetActorForwardVector(), toTarget);
		float result = FMath::RadiansToDegrees(FMath::Acos(dot));
		if (result < (AngleInDegrees * 0.5))
		{
			APawn* PawnActor = Cast<APawn>(InRange[i]);
			if (PawnActor && !HitActors.Contains(PawnActor) && Filter.FilterPassesForActor(PawnActor))
			{
				HitActors.Add(PawnActor);
			}
		}
	}

	return HitActors;
}

void ACollisionTargetActorCone::Client_Init_Implementation()
{
	if (!Sphere)
	{
		Init();
	}
}