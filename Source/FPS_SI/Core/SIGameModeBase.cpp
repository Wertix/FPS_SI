// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIGameModeBase.h"
#include "UObject/ConstructorHelpers.h"


ASIGameModeBase::ASIGameModeBase()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/Player/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
