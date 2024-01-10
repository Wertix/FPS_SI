// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SIPlayerControllerBase.generated.h"

class ASIPlayerCharacterBase;
class USIAbilitySystemComponent;


UCLASS()
class FPS_SI_API ASIPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlayingState() override;

private:

	UPROPERTY()
	TObjectPtr<ASIPlayerCharacterBase> PlayerCharacterRef;
};
