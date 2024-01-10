// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIBTT_Attack.h"

#include "FPS_SI/AI/Controllers/SIAIControllerBase.h"
#include "FPS_SI/Characters/Enemies/SIEnemyCharacterBase.h"
#include "FPS_SI/Core/FPS_SI.h"


USIBTT_Attack::USIBTT_Attack(const FObjectInitializer& ObjectInitializer)
{
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bCreateNodeInstance = true;
    NodeName = "SI Attack";
    EnemyAbilityType = EAbilityType::OffensiveMelee;
}


EBTNodeResult::Type USIBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TObjectPtr<ASIAIControllerBase> const OwnerController = Cast<ASIAIControllerBase>(OwnerComp.GetAIOwner());

    if (!ensureAlways(OwnerController))
    {
        return EBTNodeResult::Failed;
    }

    OwnerComponent = &OwnerComp;

    ControlledCharacter = Cast<ASIEnemyCharacterBase>(OwnerController->GetControlledCharacter());

    if (!ensureAlways(ControlledCharacter))
    {
        return EBTNodeResult::Failed;
    }

    GetWorld()->GetTimerManager().SetTimer(AbilityTimer, this, &USIBTT_Attack::PrepareAbilityHelper, 0.05f, false);

    ControlledCharacter->AbilityEndedDelegate.AddDynamic(this, &USIBTT_Attack::InternalFinishExecute);

    return EBTNodeResult::InProgress;
}


EBTNodeResult::Type USIBTT_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    RemoveDelegateBindings();

    return EBTNodeResult::Aborted;
}


void USIBTT_Attack::PrepareAbilityHelper()
{
    if (ControlledCharacter->PrepareAndPerformAbility(EnemyAbilityType))
    {
    }
    else
    {
        RemoveDelegateBindings();
        FinishLatentTask(*OwnerComponent, EBTNodeResult::Failed);
    }
}


void USIBTT_Attack::RemoveDelegateBindings()
{
    ControlledCharacter->AbilityEndedDelegate.RemoveDynamic(this, &USIBTT_Attack::InternalFinishExecute);
}


void USIBTT_Attack::InternalFinishExecute()
{
    RemoveDelegateBindings();
    FinishLatentTask(*OwnerComponent, EBTNodeResult::Succeeded);
}
