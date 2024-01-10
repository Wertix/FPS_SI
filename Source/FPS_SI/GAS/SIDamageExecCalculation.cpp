// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIDamageExecCalculation.h"
#include "SIAttributeSet.h"
#include "FPS_SI/Characters/Enemies/SIEnemyCharacterBase.h"


struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USIAttributeSet, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USIAttributeSet, Health, Target, false);
	}
};


static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}


USIDamageExecCalculation::USIDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}


void USIDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TObjectPtr<UAbilitySystemComponent> TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	TObjectPtr<AActor> TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	TObjectPtr<UAbilitySystemComponent> SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();
	TObjectPtr<AActor> SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	if (!ensure(TargetActor) || !ensure(SourceActor))
	{
		return;
	}

	// disable friendly fire
	if ((TargetActor == SourceActor) || TargetActor->IsA(ASIEnemyCharacterBase::StaticClass()) && SourceActor->IsA(ASIEnemyCharacterBase::StaticClass()))
	{
		return;
	}

	if (TargetABSC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Defensive.Immune"))))
	{
		return;
	}

	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, BaseDamage);
	BaseDamage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	if (BaseDamage >= 0.f)
	{
		TObjectPtr<ASICharacterBase> SourceCharacter = Cast<ASICharacterBase>(SourceActor);
		TObjectPtr<ASICharacterBase> TargetCharacter = Cast<ASICharacterBase>(SourceActor);

		if (SourceCharacter && TargetCharacter)
		{
			TargetCharacter->LastDamageCauser = SourceCharacter;
		}

		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, BaseDamage));
	}
}