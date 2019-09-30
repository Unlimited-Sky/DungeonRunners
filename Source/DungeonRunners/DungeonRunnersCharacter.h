// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "AbilitySystemInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AbilitySystemComponent.h"
#include "CharacterClassEnum.h"
#include "DungeonRunnersCharacter.generated.h"


class ADungeonRunnersGameState;
class UGlobalEventSystem;
class UAttributeSet;

//It's very important that this enum is UENUM, because the code will look for UENUM by the given name and crash if the UENUM can't be found. BlueprintType is there so we can use these in blueprints, too. Just in case. Can be neat to define ability packages.
UENUM(BlueprintType)
enum class AbilityInput : uint8
{
	UseAbility1 UMETA(DisplayName = "Use Ability 1"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.
	UseAbility2 UMETA(DisplayName = "Use Ability 2"), //Maps ability 2(input ID 1) to action mapping UseAbility2. "Use Spell 2" is mostly used for when the enum is a blueprint variable.
	UseAbility3 UMETA(DisplayName = "Use Ability 3"),
	UseAbility4 UMETA(DisplayName = "Use Ability 4"),
	UseAbility5 UMETA(DisplayName = "Use Ability 5"),
	UseAbility6 UMETA(DisplayName = "Use Ability 6"),
	UseAbility7 UMETA(DisplayName = "Use Ability 7"),
	UseAbility8 UMETA(DisplayName = "Use Ability 8")

	//You may also do something like define an enum element name that is not actually mapped to an input, for example if you have a passive ability that isn't supposed to have an input. This isn't usually necessary though as you usually grant abilities via input ID,
	//which can be negative while enums cannot. In fact, a constant called "INDEX_NONE" exists for the exact purpose of rendering an input as unavailable, and it's simply defined as -1.
	//Because abilities are granted by input ID, which is an int, you may use enum elements to describe the ID anyway however, because enums are fancily dressed up ints.
};

UCLASS(config=Game)
class ADungeonRunnersCharacter : public ACharacter, public IAbilitySystemInterface 
{
	GENERATED_BODY()

	UPROPERTY()
	UGlobalEventSystem* EventSystem;

public:
	ADungeonRunnersCharacter();

	//The Gameplay abilities component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	int MaxAbilitiesCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AbilitiesChanged, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRep_AbilitiesChanged();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Runners")
	int TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Runners")
	TSubclassOf<UAttributeSet> AttributeSetClass;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon Runners")
	//TArray<UGameplayAbility*> AbilityRefs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Runners")
	ECharacterClass CharacterClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Dungeon Runners")
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Dungeon Runners")
	//TArray<FGameplayAbilitySpec> AbilitySpecs;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	//TArray<UGameplayAbility*> AbilityActors;

	UFUNCTION()
	bool Destroy(bool bNetForce = false, bool bShouldModifyLevel = true);


protected:
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void BeginPlay();

	virtual void PossessedBy(AController* NewController) override;

	void PostInitializeComponents();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:

	UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystem;
	};

};

