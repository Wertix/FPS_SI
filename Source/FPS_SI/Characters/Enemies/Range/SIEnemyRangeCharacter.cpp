// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIEnemyRangeCharacter.h"
#include "FPS_SI/AI/Controllers/SIAIControllerBase.h"
#include "FPS_SI/Characters/Player/SIPlayerCharacterBase.h"

#include "BehaviorTree/BlackboardComponent.h"


ASIEnemyRangeCharacter::ASIEnemyRangeCharacter()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("GripPoint"));
}


void ASIEnemyRangeCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	Super::HealthChanged(Data);

	if (ensure(AIControllerRef))
	{
		if (!AIControllerRef->GetPlayerRef())
		{
			if (TObjectPtr<ASIPlayerCharacterBase> Player = Cast<ASIPlayerCharacterBase>(LastDamageCauser))
			{
				AIControllerRef->SetPlayer(Player);
			}
		}

		if (ensure(AIControllerRef->GetBlackboardComponent()) && !AIControllerRef->GetBlackboardComponent()->GetValueAsBool(TEXT("ShouldFindCover")))
		{
			AIControllerRef->GetBlackboardComponent()->SetValueAsBool(TEXT("ShouldFindCover"), true);
		}
	}
}
