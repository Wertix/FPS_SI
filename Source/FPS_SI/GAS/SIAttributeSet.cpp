// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIAttributeSet.h"
#include "FPS_SI/Characters/SICharacterBase.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"


USIAttributeSet::USIAttributeSet()
{

}


void USIAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}


void USIAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	TObjectPtr<UAbilitySystemComponent> Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	TObjectPtr<AActor> TargetActor = nullptr;
	TObjectPtr<AController> TargetController = nullptr;
	TObjectPtr<ASICharacterBase> TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ASICharacterBase>(TargetActor);
	}

	// Get the Source actor
	TObjectPtr<AActor> SourceActor = nullptr;
	TObjectPtr<AController> SourceController = nullptr;
	TObjectPtr<ASICharacterBase> SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (TObjectPtr<APawn> Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		if (SourceController)
		{
			SourceCharacter = Cast<ASICharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ASICharacterBase>(SourceActor);
		}

		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	// Damage
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			if (!TargetCharacter->IsAlive())
			{
				return;
			}

			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		}
	}// Health
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes.
		// Health loss should go through Damage.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}


void USIAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	TObjectPtr<UAbilitySystemComponent> AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
