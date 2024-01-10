// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIPlayerControllerBase.h"
#include "SIPlayerCharacterBase.h"
#include "FPS_SI/GAS/SIAbilitySystemComponent.h"


void ASIPlayerControllerBase::BeginPlayingState()
{
	Super::BeginPlayingState();

	TObjectPtr<ASIPlayerCharacterBase> PCRef = Cast<ASIPlayerCharacterBase>(GetCharacter());

	if (ensure(PCRef))
	{
		PlayerCharacterRef = PCRef;
	}
}
