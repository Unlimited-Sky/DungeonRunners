// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DungeonRunnersCharacter.h"
//#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HeadMountedDisplayTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "DungeonRunnersGameState.h"
#include "ConstructorHelpers.h"
#include "UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// ADungeonRunnersCharacter

ADungeonRunnersCharacter::ADungeonRunnersCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Our ability system component.
	MaxAbilitiesCount = 8;
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	TeamID = 0;

	CharacterClass = ECharacterClass::CC_None;
}

void ADungeonRunnersCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ADungeonRunnersCharacter, AbilityRefs);
	DOREPLIFETIME(ADungeonRunnersCharacter, AbilityHandles);
	//DOREPLIFETIME(ADungeonRunnersCharacter, AbilitySpecs);
	DOREPLIFETIME(ADungeonRunnersCharacter, Abilities);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADungeonRunnersCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADungeonRunnersCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADungeonRunnersCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADungeonRunnersCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADungeonRunnersCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADungeonRunnersCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADungeonRunnersCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADungeonRunnersCharacter::OnResetVR);

	//For AbilitySystem
	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));
}

bool ADungeonRunnersCharacter::Destroy(bool bNetForce, bool bShouldModifyLevel)
{
	AbilitySystem->TargetCancel();

	return Super::Destroy(bNetForce, bShouldModifyLevel);
}

void ADungeonRunnersCharacter::OnResetVR()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ON RESET VR HAS NO IMPLEMENTATION!"));
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADungeonRunnersCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ADungeonRunnersCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ADungeonRunnersCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADungeonRunnersCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADungeonRunnersCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADungeonRunnersCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ADungeonRunnersCharacter::BeginPlay()
{
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
		if (HasAuthority())
		{
			//AbilityHandles.Reset(MaxAbilitiesCount);
			AbilityHandles.Init(FGameplayAbilitySpecHandle(), MaxAbilitiesCount);

			for (int i = 0; i < MaxAbilitiesCount; i++)
			{
				if (Abilities[i].Get()->IsValidLowLevelFast())
				{
					FGameplayAbilitySpec SpecToAdd = FGameplayAbilitySpec(Abilities[i].GetDefaultObject(), 1, i);
					FGameplayAbilitySpecHandle Handle = AbilitySystem->GiveAbility(SpecToAdd);
					//AbilityRefs.Add(SpecToAdd.Ability);
					//AbilityHandles.Add(Handle);
					AbilityHandles[i] = Handle;
					//AbilitySpecs.Add(SpecToAdd);
					//UGameplayAbility* Ability = NewObject<UGameplayAbility>(this, Abilities[i]);
					//FGameplayAbilitySpec SpecToAdd = FGameplayAbilitySpec(Ability, 1, i);
				}
			}
		}
	}

	EventSystem = ADungeonRunnersGameState::GetDungeonRunnersGameState(GetWorld())->GetGlobalEventSystem();

	if (AttributeSetClass != NULL)
	{
		AbilitySystem->InitStats(AttributeSetClass, NULL);
	}
	else
	{
		AbilitySystem->InitStats(UBaseAttributeSet::StaticClass(), NULL);
	}

	Super::BeginPlay();
	//EventSystem->OnCharacterKilled.AddDynamic(this, &ABaseAIController::OnCharacterSeenHandler);
}

void ADungeonRunnersCharacter::PossessedBy(AController* NewController) 
{
	Super::PossessedBy(NewController);
	AbilitySystem->RefreshAbilityActorInfo();
}

void ADungeonRunnersCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADungeonRunnersCharacter::OnRep_AbilitiesChanged_Implementation()
{
	//This function is overritten in blueprints
}