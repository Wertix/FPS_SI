// FPS SI - Copyright (C) 2023 by Igor Berus.

#pragma once

#include "CoreMinimal.h"
#include "FPS_SI/Characters/SICharacterBase.h"
#include "GameFramework/Actor.h"
#include "SIProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


UCLASS(config=Game)
class ASIProjectileBase : public AActor
{
	GENERATED_BODY()

public:

	ASIProjectileBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileDamageValue;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<ASICharacterBase> ProjectileOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;


	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

private:

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;


	void DealDamageOnHit(ASICharacterBase* HitCharacter);
};
