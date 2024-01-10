// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIOffensiveAbilityBase.h"
#include "FPS_SI/Characters/Player/SIPlayerCharacterBase.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


USIOffensiveAbilityBase::USIOffensiveAbilityBase()
{
}


void USIOffensiveAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!IsActive())
    {
        CancelAbility(Handle, ActorInfo, ActivationInfo, true);

        return;
    }

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        CancelAbility(Handle, ActorInfo, ActivationInfo, true);

        return;
    }

    if (!AttackAnimMontage)
    {
        CancelAbility(Handle, ActorInfo, ActivationInfo, true);

        return;
    }

    AbilitySpecHandle = Handle;
    AbilityActorInfo = ActorInfo;
    AbilityActivationInfo = ActivationInfo;

    CreatePlayMontageAndWaitProxy(NAME_None, AttackAnimMontage, 1.f);

    WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Animation.ApplyPerformedAbilityEffects")));
}


UAbilityTask_PlayMontageAndWait* USIOffensiveAbilityBase::CreatePlayMontageAndWaitProxy(FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds)
{
    if (UAbilityTask_PlayMontageAndWait* InWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TaskInstanceName, InMontageToPlay, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale, StartTimeSeconds))
    {
        InWait->OnCompleted.AddDynamic(this, &USIOffensiveAbilityBase::MontageOnCompleted);
        InWait->OnBlendOut.AddDynamic(this, &USIOffensiveAbilityBase::MontageOnCompleted);
        InWait->OnInterrupted.AddDynamic(this, &USIOffensiveAbilityBase::MontageOnInterrupted);
        InWait->OnCancelled.AddDynamic(this, &USIOffensiveAbilityBase::MontageOnCancelled);

        InWait->ReadyForActivation();

        return InWait;
    }

    return nullptr;
}


void USIOffensiveAbilityBase::MontageOnCompleted()
{
    EndAbility(AbilitySpecHandle, AbilityActorInfo, AbilityActivationInfo, true, false);
}


void USIOffensiveAbilityBase::MontageOnCancelled()
{
    EndAbility(AbilitySpecHandle, AbilityActorInfo, AbilityActivationInfo, true, true);
}


void USIOffensiveAbilityBase::MontageOnInterrupted()
{
    EndAbility(AbilitySpecHandle, AbilityActorInfo, AbilityActivationInfo, true, true);
}


UAbilityTask_WaitGameplayEvent* USIOffensiveAbilityBase::WaitGameplayEvent(UGameplayAbility* OwningAbility, FGameplayTag EventTag, AActor* OptionalExternalTarget, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
    if (UAbilityTask_WaitGameplayEvent* InWait = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(OwningAbility, EventTag, OptionalExternalTarget, OnlyTriggerOnce, OnlyMatchExact))
    {
        InWait->EventReceived.AddDynamic(this, &USIOffensiveAbilityBase::GameplayEventReceived);

        InWait->ReadyForActivation();

        return InWait;
    }

    return nullptr;
}


void USIOffensiveAbilityBase::GameplayEventReceived(FGameplayEventData Payload)
{
    FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, 1);
    TObjectPtr<ASIPlayerCharacterBase> TargetPlayer = Cast<ASIPlayerCharacterBase>(Payload.Target);

    if (TargetPlayer && !TargetPlayer->IsAlive())
    {
        return;
    }

    // Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
    DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), DamageValue);

    ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DamageEffectSpecHandle, Payload.TargetData);
}
