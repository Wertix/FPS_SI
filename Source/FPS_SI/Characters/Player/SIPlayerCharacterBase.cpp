// FPS SI - Copyright (C) 2023 by Igor Berus.

#include "SIPlayerCharacterBase.h"
#include "SIPlayerControllerBase.h"
#include "FPS_SI/Gameplay/Components/SIWeaponComponentBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


ASIPlayerCharacterBase::ASIPlayerCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetupAttachment(FirstPersonCameraComponent);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
	GetMesh()->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	WeaponComponent = CreateDefaultSubobject<USIWeaponComponentBase>(TEXT("WeaponComponent"));
}


void ASIPlayerCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PlayerControllerRef = Cast<ASIPlayerControllerBase>(GetController());

	//Add Input Mapping Context
	if (ensure(PlayerControllerRef))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerRef->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void ASIPlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASIPlayerCharacterBase::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASIPlayerCharacterBase::Look);
	}
}


void ASIPlayerCharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (ensure(Controller))
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}


void ASIPlayerCharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (ensure(Controller))
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


bool ASIPlayerCharacterBase::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const
{
	FHitResult HitResult;
	FVector PlayerLocation = GetActorLocation();
	FVector PlayerHeadLocation = PlayerLocation + FVector(0.f, 0.f, 65.f);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, PlayerLocation, ECC_Camera, FCollisionQueryParams(SCENE_QUERY_STAT(AILineOfSight), true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHit && HitResult.GetActor() == this)
	{
		OutSeenLocation = PlayerLocation;
		OutSightStrength = 1;
		return true;
	}

	const bool bHitHead = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, PlayerHeadLocation, ECC_Camera, FCollisionQueryParams(SCENE_QUERY_STAT(AILineOfSight), true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHitHead && HitResult.GetActor() == this)
	{
		OutSeenLocation = PlayerHeadLocation;
		OutSightStrength = 1;
		return true;
	}

	OutSightStrength = 0;
	return false;
}


void ASIPlayerCharacterBase::CharacterKilled()
{
	Super::CharacterKilled();

	UGameplayStatics::OpenLevel(GetWorld(), FName("TheGame"));
}
