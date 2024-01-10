// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "../SIEnemyCharacterBase.h"
#include "SIEnemyRangeCharacter.generated.h"


UCLASS()
class FPS_SI_API ASIEnemyRangeCharacter : public ASIEnemyCharacterBase
{
	GENERATED_BODY()

public:

	ASIEnemyRangeCharacter();

protected:

	virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
