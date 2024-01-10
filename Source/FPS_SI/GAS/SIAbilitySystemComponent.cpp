// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIAbilitySystemComponent.h"
#include "FPS_SI/Characters/SICharacterBase.h"


USIAbilitySystemComponent::USIAbilitySystemComponent()
{
	SetComponentTickEnabled(false);
}


bool USIAbilitySystemComponent::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags)
{
	if (CanUseAnyAbility())
	{
		return TryActivateAbilitiesByTag(AbilityTags);
	}

	return false;
}


bool USIAbilitySystemComponent::CanUseAnyAbility() const
{
	if (TObjectPtr<ASICharacterBase> Owner = Cast<ASICharacterBase>(GetOwner()))
	{
		if (Owner->IsAlive())
		{
			return true;
		}
	}

	return false;
}
