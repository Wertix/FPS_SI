// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SIAttributeSet.generated.h"

/** Uses macros from AttributeSet.h */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class FPS_SI_API USIAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	USIAttributeSet();

	/** AttributeSet Overrides */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth.
		Positive changes can directly use this.
		Negative changes to Health should go through Damage meta attribute. */
	UPROPERTY(BlueprintReadOnly, Category = "FPS_SI|SICharacter|Attribute|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USIAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "FPS_SI|SICharacter|Attribute|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USIAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "FPS_SI|SICharacter|Attribute|CrouchSpeed")
	FGameplayAttributeData CrouchSpeed;
	ATTRIBUTE_ACCESSORS(USIAttributeSet, CrouchSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "FPS_SI|SICharacter|Attribute|WalkSpeed")
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(USIAttributeSet, WalkSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "FPS_SI|SICharacter|Attribute|RunSpeed")
	FGameplayAttributeData RunSpeed;
	ATTRIBUTE_ACCESSORS(USIAttributeSet, RunSpeed)

	/** Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "FPS_SI|SICharacter|Attribute|Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USIAttributeSet, Damage)

protected:

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
};
