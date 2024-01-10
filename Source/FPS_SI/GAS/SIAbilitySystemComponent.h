// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SIAbilitySystemComponent.generated.h"


UCLASS()
class FPS_SI_API USIAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	USIAbilitySystemComponent();

	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags);

	bool CanUseAnyAbility() const;
};
