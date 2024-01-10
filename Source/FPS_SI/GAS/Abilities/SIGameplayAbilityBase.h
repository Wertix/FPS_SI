// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SIGameplayAbilityBase.generated.h"

class ASIAIControllerBase;
class ASICharacterBase;
class ASIEnemyCharacterBase;
class ASIPlayerCharacterBase;
class ASIPlayerControllerBase;
enum class EAbilityType : uint8;


UCLASS()
class FPS_SI_API USIGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()


public:

	USIGameplayAbilityBase();

	/** Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FPS_SI|Ability")
	bool bActivateAbilityOnGranted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FPS_SI|Ability")
	EAbilityType AbilityType;


	UFUNCTION(BlueprintCallable, Category = "FPS_SI|Ability")
	void AbilityEndedHelper(UGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "FPS_SI|Ability")
	ASIPlayerControllerBase* GetSIPlayerControllerBaseFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "FPS_SI|Ability")
	ASIAIControllerBase* GetSIAIControllerBaseFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "FPS_SI|Ability")
	ASICharacterBase* GetSICharacterBaseFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "FPS_SI|Ability")
	ASIPlayerCharacterBase* GetSIPlayerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "FPS_SI|Ability")
	ASIEnemyCharacterBase* GetSIEnemyCharacterBaseFromActorInfo() const;

	/** If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here */
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
