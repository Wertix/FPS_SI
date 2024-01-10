// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "../SIGameplayAbilityBase.h"
#include "SIOffensiveAbilityBase.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;


UCLASS()
class FPS_SI_API USIOffensiveAbilityBase : public USIGameplayAbilityBase
{
	GENERATED_BODY()
	

public:

	USIOffensiveAbilityBase();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Offensive ability")
	TObjectPtr<UAnimMontage> AttackAnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Offensive ability")
	float DamageValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Offensive ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;


	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	UAbilityTask_PlayMontageAndWait* CreatePlayMontageAndWaitProxy(FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);
	UFUNCTION(BlueprintCallable)
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent(UGameplayAbility* OwningAbility, FGameplayTag EventTag, AActor* OptionalExternalTarget = nullptr, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);

protected:

	FGameplayAbilitySpecHandle AbilitySpecHandle;
	const FGameplayAbilityActorInfo* AbilityActorInfo;
	FGameplayAbilityActivationInfo AbilityActivationInfo;


	UFUNCTION()
	void GameplayEventReceived(FGameplayEventData Payload);

private:

	FTimerHandle AnimMontageTimerHandle;


	UFUNCTION()
	void MontageOnCompleted();
	UFUNCTION()
	void MontageOnCancelled();
	UFUNCTION()
	void MontageOnInterrupted();
};
