// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIEnemyCharacterBase.h"
#include "FPS_SI/AI/Controllers/SIAIControllerBase.h"
#include "FPS_SI/AI/Spawners/SISpawnerBase.h"
#include "FPS_SI/Characters/SIAnimInstance.h"
#include "FPS_SI/GAS/Abilities/SIGameplayAbilityBase.h"
#include "FPS_SI/GAS/SIAbilitySystemComponent.h"
#include "FPS_SI/GAS/SIAttributeSet.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASIEnemyCharacterBase::ASIEnemyCharacterBase()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	bCanBeRevived = true;
}


void ASIEnemyCharacterBase::CharacterKilled()
{
	EnemyDiedDelegate.Broadcast();

	if (ensure(SIAnimInstance) && EnemySettings.DeathAnimationMontage)
	{
		if (ensure(AIControllerRef))
		{
			AIControllerRef->SetPlayer(nullptr);
			AIControllerRef->StopBehaviorTree();
		}

		SIAnimInstance->Montage_Play(EnemySettings.DeathAnimationMontage);

		GetWorldTimerManager().SetTimer(DeadBodyTimer, this, &ASIEnemyCharacterBase::DestroyAfterDelay, EnemySettings.DeadBodyDelay, false);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		Destroy();
	}
}


void ASIEnemyCharacterBase::DestroyAfterDelay()
{
	Destroy();
}


bool ASIEnemyCharacterBase::PrepareAndPerformAbility(EAbilityType AbilityTypeToPerform)
{
	if (!IsAlive())
	{
		return false;
	}

	if (GameplayAbilities.Num() > 0 && ensure(AbilitySystemComponent))
	{
		TArray<TSubclassOf<USIGameplayAbilityBase>> Abilities = GameplayAbilities;

		for (auto Ability : GameplayAbilities)
		{
			if (ensure(Ability->IsValidLowLevel()))
			{
				PerformedAbility = NewObject<USIGameplayAbilityBase>(this, Ability);

				if (ensure(PerformedAbility) && PerformedAbility->AbilityType == AbilityTypeToPerform)
				{
					return AbilitySystemComponent->TryActivateAbilityByClass(Ability, true);
				}
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}


void ASIEnemyCharacterBase::SetRevivingMedicRef(ASIEnemyCharacterBase* NewRevivingMedicRef)
{
	RevivingMedicRef = NewRevivingMedicRef;

	if (RevivingMedicRef && bCanBeRevived)
	{
		GetWorldTimerManager().SetTimer(DeadBodyTimer, this, &ASIEnemyCharacterBase::DestroyAfterDelay, 10.f, false);

		RevivingMedicRef->EnemyDiedDelegate.AddUniqueDynamic(this, &ASIEnemyCharacterBase::ClearRevivingMedicRef);
	}
}


void ASIEnemyCharacterBase::ClearRevivingMedicRef()
{
	SetRevivingMedicRef(nullptr);
}


void ASIEnemyCharacterBase::ReviveCharacter()
{
	bCanBeRevived = false;

	if (ensure(SIAnimInstance))
	{
		SIAnimInstance->StopAllMontages(0.25f);

		GetWorldTimerManager().ClearTimer(DeadBodyTimer);

		if (AIControllerRef)
		{
			AIControllerRef->StartBehaviorTree();
		}

		Attributes->SetHealth(Attributes->GetMaxHealth());

		SpawnerRef->IncrementSpawnerCounter();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
