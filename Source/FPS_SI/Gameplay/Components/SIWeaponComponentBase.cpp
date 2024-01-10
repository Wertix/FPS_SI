// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIWeaponComponentBase.h"

#include "AbilitySystemComponent.h"
#include "FPS_SI/Characters/Player/SIPlayerCharacterBase.h"

#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPS_SI/GAS/SIAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"


USIWeaponComponentBase::USIWeaponComponentBase()
{

}


void USIWeaponComponentBase::Fire()
{
	if (!ensure(WeaponOwner) || !ensure(WeaponOwner->AbilitySystemComponent))
	{
		return;
	}

	FGameplayTagContainer TagContainer = FGameplayTagContainer();
	TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Offensive.Range")));

	WeaponOwner->AbilitySystemComponent->ActivateAbilitiesWithTags(TagContainer);
}


void USIWeaponComponentBase::AttachWeapon(ASIPlayerCharacterBase* TargetCharacter)
{
	WeaponOwner = TargetCharacter;

	if (!ensure(WeaponOwner))
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(WeaponOwner->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));

	// Set up action bindings
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(WeaponOwner->GetController()))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &USIWeaponComponentBase::Fire);
		}
	}
}


void USIWeaponComponentBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!ensure(WeaponOwner))
	{
		return;
	}

	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(WeaponOwner->GetController()))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
