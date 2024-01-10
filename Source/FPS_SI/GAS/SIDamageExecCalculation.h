// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SIDamageExecCalculation.generated.h"


UCLASS()
class FPS_SI_API USIDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	USIDamageExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
