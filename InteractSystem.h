// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "DoorComponent.h"
#include "InteractSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINE_API UInteractSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual AActor* GetActorInView();

	UFUNCTION(BlueprintCallable, Category="UI")
	virtual void CloseDoorLockUI();

	UFUNCTION(BlueprintCallable, Category="Interaction")
	virtual void DetachHeldActor();
private:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceDistance = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	FName SmallPickUpSocketName = "PickUp_Small";

	UPROPERTY(EditAnywhere, Category = "Interaction")
	FName BigPickUpSocketName = "PickUp_Big";

	UUserWidget* DoorLockWidget;
	UUserWidget* DoorHintWidget;
	UUserWidget* LockHintWidget;
	ACharacter* ControlledCharacter;
	UPROPERTY()
	AActor* GarbageActor;
	void DoorHintUICreateAndRemove();
	void LockHintUICreateAndRemove();
	void UpdateOutlineEffect(AActor* TargetActor);

	bool bIsOpening;
	FTimerHandle TimerHandle;
public:

	//UPROPERTY(EditAnywhere, Categroy = "InteractionUI")
	//TSubclassOf<UUserWidget> InteractWidgetClass;

	UUserWidget* InteractWidget;
	AActor* CurrentTarget;
	AActor* CurrentOutlinerActor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction")
	AActor* HeldActor;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bIsHolding;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bIsHoldingSmall;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bIsOpeningDoor;
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bIsClosingDoor;
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bIsOpeningLocker;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DoorLockWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DoorHintWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LockHintWidgetClass;
};
