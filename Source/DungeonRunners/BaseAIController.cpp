// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include "DungeonRunnersGameState.h"
#include "AbilitySystemComponent.h"

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	GlobalEventSystem = ADungeonRunnersGameState::GetDungeonRunnersGameState(GetWorld())->GetGlobalEventSystem();
	GlobalEventSystem->OnEffectThreat.AddDynamic(this, &ABaseAIController::OnEffectThreatHandler);
	GlobalEventSystem->OnCharacterSeen.AddDynamic(this, &ABaseAIController::OnCharacterSeenHandler);
}

void ABaseAIController::OnEffectThreatHandler(float ThreatAmount, ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target)
{
	ADungeonRunnersCharacter* myPawn = Cast<ADungeonRunnersCharacter>(GetPawn());
	bool KnownTarget = ThreatMap.Contains(Target);

	if (!myPawn || !Source || !Target)
		return;

	//Was it from another team? Otherwise we don't care
	if (myPawn->TeamID != Source->TeamID)
	{
		if (myPawn == Target || //Targetted me or
			(KnownTarget && 
			Target->TeamID == Source->TeamID)) //They are on a team helping each other, and we know the target
		{
			if (ThreatMap.Contains(Source)) {
				ThreatMap[Source] += ThreatAmount;
			} else {
				ThreatMap.Add(Source, ThreatAmount);
			}
			SortThreatMap();
		}
	}
}

void ABaseAIController::SortThreatMap() 
{
	ThreatMap.ValueSort([](const float A, const float B) {
		return A > B;
	});

	ThreatMap.GenerateValueArray(ThreatValuesSorted);
	ThreatMap.GenerateKeyArray(ThreatCharactersSorted);
}

void ABaseAIController::OnCharacterSeenHandler(ADungeonRunnersCharacter* Source, ADungeonRunnersCharacter* Target)
{
}

void ABaseAIController::FindTopThreat()
{
	if (ThreatCharactersSorted.Num() > 0) {
		CurrentTarget = ThreatCharactersSorted[0];
	}
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UAbilitySystemComponent* PawnsComponent = (Cast<ADungeonRunnersCharacter>(GetPawn()))->GetAbilitySystemComponent();
	PawnsComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ABaseAIController::OnEffectAppliedSelf);
	
	FOnGameplayEffectTagCountChanged& TauntCountChanged = PawnsComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName(TEXT("AbilityTags.Gameplay.Taunt"))));
	TauntCountChanged.AddUObject(this, &ABaseAIController::OnTauntCountChanged);
}

void ABaseAIController::OnTaunt(ADungeonRunnersCharacter* Taunter)
{
	float CurrentMaxThreat = 0.f;

	if (ThreatValuesSorted.IsValidIndex(0))
	{	
		CurrentMaxThreat = ThreatValuesSorted[0];
	}

	//float ThreatDifference = CurrentMaxThreat - ThreatMap[Taunter];
	bIsTaunted = true;
	CurrentTarget = Taunter;
	ThreatMap.FindOrAdd(Taunter) = CurrentMaxThreat + 0.1f;
	
	SortThreatMap();
}

bool ABaseAIController::TryAddCharacterToThreatMap(ADungeonRunnersCharacter* Target)
{
	if (ThreatMap.Contains(Target))
		return false;

	ThreatMap.Add(Target, ThreatMap.Num() * -0.1f);
	SortThreatMap();

	return true;
}

void ABaseAIController::OnTauntCountChanged(const FGameplayTag Tag, int32 Count)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FString("taunt count changed"));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(Count));
	if (Tag == FGameplayTag::RequestGameplayTag(FName(TEXT("AbilityTags.Gameplay.Taunt"))))
	{
		if (Count == 0)
		{
			bIsTaunted = false;
		}
	}
}

void ABaseAIController::OnEffectAppliedSelf(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (GrantedTags.HasTag(FGameplayTag::RequestGameplayTag(FName(TEXT("AbilityTags.Gameplay.Taunt")), false)))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FString("Taunt applied"));
		OnTaunt(Cast<ADungeonRunnersCharacter>(Source->GetOwner()));
	}
}