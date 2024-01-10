// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SICharacterBase.h"
#include "SIAnimInstance.h"
#include "FPS_SI/Core/FPS_SI.h"
#include "FPS_SI/GAS/Abilities/SIGameplayAbilityBase.h"
#include "FPS_SI/GAS/SIAbilitySystemComponent.h"
#include "FPS_SI/GAS/SIAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASICharacterBase::ASICharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 96.0f);

	AbilitySystemComponent = CreateDefaultSubobject<USIAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Attributes = CreateDefaultSubobject<USIAttributeSet>(TEXT("Attributes"));
}


void ASICharacterBase::BeginPlay()
{
	Super::BeginPlay();

    if (TObjectPtr<USkeletalMeshComponent> SkeletalMesh = GetMesh())
    {
        SIAnimInstance = Cast<USIAnimInstance>(SkeletalMesh->GetAnimInstance());
    }

	if (ensure(AbilitySystemComponent) && ensure(Attributes))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddCharacterAbilities();
		AddStartupEffects();
		BindAttributeChangeCallbacks();

		GetCharacterMovement()->MaxWalkSpeed = Attributes->GetRunSpeed();
	}
}


UAbilitySystemComponent* ASICharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ASICharacterBase::BindAttributeChangeCallbacks()
{
	if (ensure(AbilitySystemComponent) && ensure(Attributes))
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this, &ASICharacterBase::HealthChanged);
	}
}


void ASICharacterBase::InitializeAttributes()
{
	if (!ensure(AbilitySystemComponent))
	{
		return;
	}

	if (!DefaultAttributeEffects)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
	}
}


void ASICharacterBase::AddCharacterAbilities()
{
	if (!AbilitySystemComponent || bAbilitiesInitialized)
	{
		return;
	}

	for (TSubclassOf<USIGameplayAbilityBase>& StartupAbility : GameplayAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, 0, this));

#if !UE_BUILD_SHIPPING
		if (TObjectPtr<USIGameplayAbilityBase> Ability = Cast<USIGameplayAbilityBase>(StartupAbility->GetDefaultObject()))
		{
			if (Ability->AbilityType == EAbilityType::None)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s has missing ability type."), *Ability->GetName()));
			}
		}
#endif
	}

	bAbilitiesInitialized = true;
}


void ASICharacterBase::AddStartupEffects()
{
	if (!AbilitySystemComponent || bStartupEffectsInitialized)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}

	bStartupEffectsInitialized = true;
}


void ASICharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.OldValue > Data.NewValue)
	{
		if (!IsAlive())
		{
			CharacterKilled();
		}
	}
}


bool ASICharacterBase::IsAlive() const
{
	if (Attributes)
	{
		return Attributes->GetHealth() > 0.0f;
	}

	return false;
}


void ASICharacterBase::CharacterKilled()
{
}


void ASICharacterBase::CallAbilityEndedDelegateHelper()
{
	AbilityEndedDelegate.Broadcast();
}
