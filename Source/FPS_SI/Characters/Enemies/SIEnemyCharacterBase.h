// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "../SICharacterBase.h"
#include "SIEnemyCharacterBase.generated.h"

enum class EAbilityType : uint8;
class ASIAIControllerBase;
class ASISpawnerBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDied);


USTRUCT(BlueprintType)
struct FSIEnemySettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SightRadius = 3500.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float VisionHalfAngle = 180.f;

	/** Negative value means not applicable. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinDistFromPlayer = -1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxDistFromPlayer = -1.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimMontage> DeathAnimationMontage;

	/** How long should corpse lay on the ground */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float DeadBodyDelay = 10.f;
};

UCLASS()
class FPS_SI_API ASIEnemyCharacterBase : public ASICharacterBase
{
	GENERATED_BODY()

public:

	ASIEnemyCharacterBase();

	FEnemyDied EnemyDiedDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Settings")
	FSIEnemySettings EnemySettings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanBeRevived;


	ASIAIControllerBase* GetAIControllerRef() const { return AIControllerRef; }
	void SetAIControllerRef(ASIAIControllerBase* NewAIControllerRef) { AIControllerRef = NewAIControllerRef; }

	ASISpawnerBase* GetSpawnerRef() const { return SpawnerRef; }
	void SetSpawnerRef(ASISpawnerBase* NewSpawnerRef) { SpawnerRef = NewSpawnerRef; }

	UFUNCTION(BlueprintCallable)
	ASIEnemyCharacterBase* GetRevivingMedicRef() const { return RevivingMedicRef; }
	UFUNCTION(BlueprintCallable)
	void SetRevivingMedicRef(ASIEnemyCharacterBase* NewRevivingMedicRef);
	UFUNCTION()
	void ClearRevivingMedicRef();

	/** Find ability, set pointer to it and play animation */
	bool PrepareAndPerformAbility(EAbilityType AbilityTypeToPerform);

	UFUNCTION(BlueprintCallable)
	void ReviveCharacter();

protected:

	UPROPERTY()
	TObjectPtr<ASIAIControllerBase> AIControllerRef;


	virtual void CharacterKilled() override;

private:

	UPROPERTY()
	TObjectPtr<ASISpawnerBase> SpawnerRef;

	UPROPERTY()
	TObjectPtr<USIGameplayAbilityBase> PerformedAbility;

	FTimerHandle DeadBodyTimer;

	UPROPERTY()
	TObjectPtr<ASIEnemyCharacterBase> RevivingMedicRef;

	void DestroyAfterDelay();
};
