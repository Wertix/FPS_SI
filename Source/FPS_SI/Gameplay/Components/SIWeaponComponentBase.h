// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SIWeaponComponentBase.generated.h"

class ASIPlayerCharacterBase;
class ASIProjectileBase;
class UInputAction;
class UInputMappingContext;


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_SI_API USIWeaponComponentBase : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	USIWeaponComponentBase();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireAction;

	
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(ASIPlayerCharacterBase* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

protected:

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY()
	TObjectPtr<ASIPlayerCharacterBase> WeaponOwner;
};
