// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None,
	OffensiveMelee,
	OffensiveRange,
	Buff,
	MAX
};