// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineCharacter.h"
#include "MineProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMineCharacter

void AMineCharacter::StartCrouch()
{
	Crouch();
}

void AMineCharacter::StopCrouch()
{
	UnCrouch();
}

AMineCharacter::AMineCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	InteractSystem = CreateDefaultSubobject<UInteractSystem>(TEXT("InteractSystem"));

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AMineCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->EnableInput(PlayerController);
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	
}

//////////////////////////////////////////////////////////////////////////// Input

void AMineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMineCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMineCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMineCharacter::OnInteract);

		//Detaching
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AMineCharacter::OnDetachHeldActor);
		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMineCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMineCharacter::StopCrouch);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMineCharacter::OnInteract()
{
	if (InteractSystem)
	{
		InteractSystem->Interact();
		return;
	}
	else
	{
		return;
	}
}

void AMineCharacter::OnDetachHeldActor()
{
	if (InteractSystem)
	{
		InteractSystem->DetachHeldActor();
	}
	else
	{
		return;
	}
}

bool AMineCharacter::GetIsHoldingItem() const
{
	return InteractSystem ? InteractSystem->bIsHolding : false;
}

bool AMineCharacter::GetIsHoldingItemSmall() const
{
	return InteractSystem ? InteractSystem->bIsHoldingSmall : false;
}

bool AMineCharacter::GetIsOpeningDoor() const
{
	return InteractSystem ? InteractSystem->bIsOpeningDoor : false;
}

bool AMineCharacter::GetIsClosingDoor() const
{
	return InteractSystem ? InteractSystem->bIsClosingDoor : false;
}

bool AMineCharacter::GetIsOpeningLocker() const
{
	return InteractSystem ? InteractSystem->bIsOpeningLocker : false;
}

void AMineCharacter::SetIsOpeingDoor(bool bNewState)
{
	if (InteractSystem)
	{
		InteractSystem->bIsOpeningDoor = bNewState;
	}
}

void AMineCharacter::SetIsClosingDoor(bool bNewState)
{
	if (InteractSystem)
	{
		InteractSystem->bIsClosingDoor = bNewState;
	}
}

void AMineCharacter::SetIsOpeingLocker(bool bNewState)
{
	if (InteractSystem)
	{
		InteractSystem->bIsOpeningLocker = bNewState;
	}
}


void AMineCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMineCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * MouseSensivity);
		AddControllerPitchInput(LookAxisVector.Y * MouseSensivity);
	}
}