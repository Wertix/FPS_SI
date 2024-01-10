// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SIAIControllerBase.generated.h"

struct FAIStimulus;
class ASIEnemyCharacterBase;
class ASIPlayerCharacterBase;
class UAISenseConfig_Sight;
class UBehaviorTreeComponent;


UCLASS()
class FPS_SI_API ASIAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:

	ASIAIControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAISenseConfig_Sight> AISightConfig;


	void StartBehaviorTree();
	void StopBehaviorTree();

	ASIEnemyCharacterBase* GetControlledCharacter() const { return ControlledCharacter; }
	UFUNCTION(BlueprintCallable)
	ASIPlayerCharacterBase* GetPlayerRef() const { return PlayerRef; }

	void SetPlayer(ASIPlayerCharacterBase* NewPlayer);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;


	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

private:

	static constexpr auto PlayerKeyName = TEXT("Player");

	UPROPERTY()
	TObjectPtr<ASIEnemyCharacterBase> ControlledCharacter;
	UPROPERTY()
	TObjectPtr<ASIPlayerCharacterBase> PlayerRef;
};
