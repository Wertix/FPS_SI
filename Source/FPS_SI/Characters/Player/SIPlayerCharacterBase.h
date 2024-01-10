// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "../SICharacterBase.h"
#include "Perception/AISightTargetInterface.h"
#include "SIPlayerCharacterBase.generated.h"

class UAnimMontage;
class ASIPlayerControllerBase;
class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class USceneComponent;
class USIWeaponComponentBase;
class USkeletalMeshComponent;
class USoundBase;
struct FInputActionValue;


UCLASS()
class FPS_SI_API ASIPlayerCharacterBase : public ASICharacterBase, public IAISightTargetInterface
{
	GENERATED_BODY()

public:

	ASIPlayerCharacterBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USIWeaponComponentBase> WeaponComponent;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;


	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	ASIPlayerControllerBase* GetPlayerControllerRef() const { return PlayerControllerRef; }

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = nullptr, const bool* bWasVisible = nullptr, int32* UserData = nullptr) const override;

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void CharacterKilled() override;

private:

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY()
	TObjectPtr<ASIPlayerControllerBase> PlayerControllerRef;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
};
