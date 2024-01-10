// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIProjectileBase.h"
#include "FPS_SI/Characters/SICharacterBase.h"
#include "FPS_SI/GAS/SIAbilitySystemComponent.h"

#include "GameplayEffectTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"


ASIProjectileBase::ASIProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASIProjectileBase::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.4f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	ProjectileDamageValue = 15.f;
}


void ASIProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == Owner)
	{
		Destroy();
		return;
	}

	if (TObjectPtr<ASICharacterBase> HitCharacter = Cast<ASICharacterBase>(OtherActor))
	{
		DealDamageOnHit(HitCharacter);
	}

	Destroy();
}


void ASIProjectileBase::DealDamageOnHit(ASICharacterBase* HitCharacter)
{
	TObjectPtr<USIAbilitySystemComponent> AbilitySystemComponent = HitCharacter->AbilitySystemComponent;

	if (!ensure(AbilitySystemComponent))
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	EffectContext.AddInstigator(ProjectileOwner, this);

	FGameplayEffectSpecHandle DamageEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageGameplayEffect, 1, EffectContext);
	// Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), ProjectileDamageValue);

	if (DamageEffectSpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	}
}

