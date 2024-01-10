// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIGameplayAbilityBase.h"
#include "FPS_SI/Characters/SICharacterBase.h"
#include "FPS_SI/Characters/Enemies/SIEnemyCharacterBase.h"
#include "FPS_SI/Characters/Player/SIPlayerCharacterBase.h"
#include "FPS_SI/Characters/Player/SIPlayerControllerBase.h"
#include "FPS_SI/AI/Controllers/SIAIControllerBase.h"

#include "AbilitySystemComponent.h"


USIGameplayAbilityBase::USIGameplayAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
}


void USIGameplayAbilityBase::AbilityEndedHelper(UGameplayAbility* Ability)
{
	if (TObjectPtr<ASICharacterBase> Character =  GetSICharacterBaseFromActorInfo())
	{
		Character->CallAbilityEndedDelegateHelper();
	}
}


ASIPlayerControllerBase* USIGameplayAbilityBase::GetSIPlayerControllerBaseFromActorInfo() const
{
	TObjectPtr<ASIPlayerControllerBase> PC = nullptr;

	if (TObjectPtr<ASIPlayerCharacterBase> Palyer = GetSIPlayerFromActorInfo())
	{
		PC = Cast<ASIPlayerControllerBase>(Palyer->GetPlayerControllerRef());
	}

	return PC;
}


ASIAIControllerBase* USIGameplayAbilityBase::GetSIAIControllerBaseFromActorInfo() const
{
	TObjectPtr<ASIAIControllerBase> AIC = nullptr;

	if (TObjectPtr<ASIEnemyCharacterBase> AIChar = GetSIEnemyCharacterBaseFromActorInfo())
	{
		AIC = Cast<ASIAIControllerBase>(AIChar->GetAIControllerRef());
	}

	return AIC;
}


ASICharacterBase* USIGameplayAbilityBase::GetSICharacterBaseFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASICharacterBase>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}


ASIPlayerCharacterBase* USIGameplayAbilityBase::GetSIPlayerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASIPlayerCharacterBase>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}


ASIEnemyCharacterBase* USIGameplayAbilityBase::GetSIEnemyCharacterBaseFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASIEnemyCharacterBase>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}


void USIGameplayAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted)
	{
		bool bActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}


void USIGameplayAbilityBase::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AbilityEndedHelper(this);
}


void USIGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AbilityEndedHelper(this);
}
