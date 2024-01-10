// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "SICharacterBase.generated.h"

class UGameplayEffect;
class USIAbilitySystemComponent;
class USIAnimInstance;
class USIAttributeSet;
class USIGameplayAbilityBase;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityEndedHelper);


UCLASS(config=Game)
class ASICharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ASICharacterBase();

	UPROPERTY()
	TObjectPtr<USIAttributeSet> Attributes;

	UPROPERTY()
	TObjectPtr<USIAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY();
	FAbilityEndedHelper AbilityEndedDelegate;

	UPROPERTY();
	TObjectPtr<ASICharacterBase> LastDamageCauser;


	/** IAbilitySystemInterface. Do not delete */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void BindAttributeChangeCallbacks();

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

	void CallAbilityEndedDelegateHelper();

protected:

	virtual void BeginPlay() override;

	void InitializeAttributes();
	void AddCharacterAbilities();
	virtual void AddStartupEffects();


	UPROPERTY()
	TObjectPtr<USIAnimInstance> SIAnimInstance;

	/** If true, abilities are initialized */
	UPROPERTY()
	bool bAbilitiesInitialized;
	/** If true, StartupEffects are initialized  */
	UPROPERTY()
	bool bStartupEffectsInitialized;

	/** Effect that initializes default attributes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS_SI|Ability")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffects;

	/** Abilities to grant to this character on creation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS_SI|Ability")
	TArray<TSubclassOf<USIGameplayAbilityBase>> GameplayAbilities;

	/** These effects are only applied one time on startup */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FPS_SI|Ability")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	FDelegateHandle HealthChangedDelegateHandle;


	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable)
	virtual void CharacterKilled();
};
