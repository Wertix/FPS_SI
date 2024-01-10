// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SIBTT_Attack.generated.h"


enum class EAbilityType : uint8;
class ASIEnemyCharacterBase;


UCLASS()
class FPS_SI_API USIBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	USIBTT_Attack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAbilityType EnemyAbilityType;

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> OwnerComponent;
	UPROPERTY()
	TObjectPtr<ASIEnemyCharacterBase> ControlledCharacter;

	/* Handle to manage the timer. */
	FTimerHandle AbilityTimer;


	UFUNCTION()
	void PrepareAbilityHelper();

	void RemoveDelegateBindings();

	UFUNCTION()
	void InternalFinishExecute();
};
