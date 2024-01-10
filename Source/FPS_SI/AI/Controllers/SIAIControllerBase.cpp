// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIAIControllerBase.h"
#include "FPS_SI/Characters/Enemies/SIEnemyCharacterBase.h"
#include "FPS_SI/Characters/Player/SIPlayerCharacterBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ASIAIControllerBase::ASIAIControllerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));

	PerceptionComponent->ConfigureSense(*AISightConfig);
	PerceptionComponent->SetDominantSense(AISightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASIAIControllerBase::OnTargetPerceptionUpdated);

	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
}


void ASIAIControllerBase::StartBehaviorTree()
{
	if (ensure(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);

		if (ensure(Blackboard))
		{
			Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		}

		PerceptionComponent->ForgetAll();
	}
}


void ASIAIControllerBase::StopBehaviorTree()
{
	if (BrainComponent)
	{
		FString Reason;
		BrainComponent->StopLogic(Reason);
	}
}


void ASIAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledCharacter = Cast<ASIEnemyCharacterBase>(InPawn);

	if (ensureAlways(ControlledCharacter))
	{
		ControlledCharacter->SetAIControllerRef(this);

		if (ensureAlways(AISightConfig))
		{
			AISightConfig->SightRadius = ControlledCharacter->EnemySettings.SightRadius;
			AISightConfig->LoseSightRadius = ControlledCharacter->EnemySettings.SightRadius * 1.2f;
			AISightConfig->PeripheralVisionAngleDegrees = ControlledCharacter->EnemySettings.VisionHalfAngle;
			AISightConfig->DetectionByAffiliation.bDetectEnemies = true;
			AISightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			AISightConfig->DetectionByAffiliation.bDetectFriendlies = true;
			PerceptionComponent->ConfigureSense(*AISightConfig);

			UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, AISightConfig->GetSenseImplementation(), ControlledCharacter);
		}

		StartBehaviorTree();
	}
}


void ASIAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

	PerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
}


void ASIAIControllerBase::OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	if (UpdatedActor->IsA(ASIPlayerCharacterBase::StaticClass()) && UpdatedActor != PlayerRef)
	{
		if (Stimulus.WasSuccessfullySensed() && ensure(ControlledCharacter) && ControlledCharacter->IsAlive())
		{
			if (PlayerRef = Cast<ASIPlayerCharacterBase>(UpdatedActor))
			{
				SetPlayer(PlayerRef);
			}
		}
		else
		{
			if (ensure(Blackboard))
			{
				SetPlayer(nullptr);
			}
		}
	}
}


void ASIAIControllerBase::SetPlayer(ASIPlayerCharacterBase* NewPlayer)
{
	if (NewPlayer)
	{
		SetFocus(NewPlayer);
		Blackboard->SetValueAsObject(PlayerKeyName, NewPlayer);	
	}
	else
	{
		Blackboard->ClearValue(PlayerKeyName);
		ClearFocus(EAIFocusPriority::LastFocusPriority);
		PlayerRef = nullptr;
	}
}
