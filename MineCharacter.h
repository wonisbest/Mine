// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Public/InteractSystem.h"
#include "MineCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInteractSystem;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMineCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/* Interact Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	/* Crouch Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropAction;

	UFUNCTION(BlueprintCallable, category="Movement")
	void StartCrouch();
	UFUNCTION(BlueprintCallable, category = "Movement")
	void StopCrouch();
public:
	AMineCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MouseSensivity = 1.0f;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	UInteractSystem* InteractSystem;

	UFUNCTION()
	void OnInteract();
	UFUNCTION()
	void OnDetachHeldActor();
public:
	//Anim BluePrint에서 사용할 Getter 함수
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool GetIsHoldingItem() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetIsHoldingItemSmall() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetIsOpeningDoor() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetIsClosingDoor() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetIsOpeningLocker() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetIsOpeingDoor(bool bNewState);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetIsClosingDoor(bool bNewState);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetIsOpeingLocker(bool bNewState);
};

