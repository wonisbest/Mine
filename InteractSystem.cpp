// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractSystem.h"
#include "Components/StaticMeshComponent.h"
// Sets default values for this component's properties
UInteractSystem::UInteractSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UInteractSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GarbageActor = GetActorInView();
	
	// ...
}

void UInteractSystem::Interact()
{

	AActor* HitActor = GetActorInView();
	FName Tag;
	if (HitActor && HitActor->ActorHasTag("DoorLocker") && DoorLockWidgetClass)
	{
		if (!DoorLockWidget)
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				ControlledCharacter = Cast<ACharacter>(PlayerController->GetPawn());
				DoorLockWidget = CreateWidget<UUserWidget>(PlayerController, DoorLockWidgetClass);
				if (DoorLockWidget)
				{
					ControlledCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
					ControlledCharacter->GetCharacterMovement()->DisableMovement();
					DoorLockWidget->AddToViewport();
					FInputModeUIOnly InputMode;
					PlayerController->SetInputMode(InputMode);
					PlayerController->bShowMouseCursor = true;
					//ControlledCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				}
			}
		}
	}
	if (bIsHolding && HeldActor)
	{
		UStaticMeshComponent* HeldComponent = HeldActor->FindComponentByClass<UStaticMeshComponent>();
		if (HitActor && HitActor->Tags.Num() > 0 && HitActor->ActorHasTag("Locker"))
		{
			if (HeldActor->ActorHasTag("Key"))
			{
				if (HeldActor->ActorHasTag("Key_1") && HitActor->ActorHasTag("Locker_1"))
				{
					HeldComponent->SetSimulatePhysics(true);
					//HeldComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					HeldComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
					HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					HitActor->Destroy();
					HeldActor->Destroy();
					HeldActor = nullptr;
					bIsHolding = false;
					bIsHoldingSmall = false;
					bIsOpeningLocker = true;
					Tag = "";
					return;
				}
				else if (HeldActor->ActorHasTag("Key_2") && HitActor->ActorHasTag("Locker_2"))
				{
					HeldComponent->SetSimulatePhysics(true);
					//HeldComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					HeldComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
					HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					HitActor->Destroy();
					HeldActor->Destroy();
					HeldActor = nullptr;
					bIsHolding = false;
					bIsHoldingSmall = false;
					bIsOpeningLocker = true;
					Tag = "";
					return;
				}
				else if (HeldActor->ActorHasTag("Key_3") && HitActor->ActorHasTag("Locker_3"))
				{
					HeldComponent->SetSimulatePhysics(true);
					//HeldComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					HeldComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
					HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					HitActor->Destroy();
					HeldActor->Destroy();
					HeldActor = nullptr;
					bIsHolding = false;
					bIsHoldingSmall = false;
					bIsOpeningLocker = true;
					Tag = "";
					return;
				}
				else if (HeldActor->ActorHasTag("Key_1") && (HitActor->ActorHasTag("Locker_2") || HitActor->ActorHasTag("Locker_3")))
				{
					bIsOpeningLocker = true;
					LockHintUICreateAndRemove();
					return;
				}
				else if (HeldActor->ActorHasTag("Key_2") && (HitActor->ActorHasTag("Locker_1") || HitActor->ActorHasTag("Locker_3")))
				{
					bIsOpeningLocker = true;
					LockHintUICreateAndRemove();
					return;
				}
				else if (HeldActor->ActorHasTag("Key_3") && (HitActor->ActorHasTag("Locker_1") || HitActor->ActorHasTag("Locker_2")))
				{
					bIsOpeningLocker = true;
					LockHintUICreateAndRemove();
					return;
				}
				
			}
		}
		
	}
	

	if (HitActor)
	{
		AActor* OwnerActor = GetOwner();
		USkeletalMeshComponent* GrabMeshComponent = nullptr;


		if (OwnerActor)
		{
			TArray<UActorComponent*> Components;
			OwnerActor->GetComponents(Components);
			for (UActorComponent* Comp : Components)
			{
				USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Comp);
				if (SkeletalMesh && SkeletalMesh->ComponentHasTag(FName("GrabMesh")))
				{
					GrabMeshComponent = SkeletalMesh;
				}
			}
		}

		if (HitActor->Tags.Num() > 0)
		{
			Tag = HitActor->Tags[0];
			//물체를 집고 있는 경우
			if (bIsHolding && Tag.ToString() == "PickUp_Small")
			{
				DetachHeldActor();
				UStaticMeshComponent* HitComponent = HitActor->FindComponentByClass<UStaticMeshComponent>();
				if (HitComponent)
				{
					HitComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
					HitComponent->SetSimulatePhysics(false);
				}
				HitActor->AttachToComponent(GrabMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SmallPickUpSocketName);
				HeldActor = HitActor;
				bIsHolding = true;
				bIsHoldingSmall = true;
				return;
			}
			else if (bIsHolding && Tag.ToString() == "PickUp_Big")
			{
				DetachHeldActor();
				UStaticMeshComponent* HitComponent = HitActor->FindComponentByClass<UStaticMeshComponent>();
				if (HitComponent)
				{
					HitComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
					HitComponent->SetSimulatePhysics(false);
				}
				HitActor->AttachToComponent(GrabMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, BigPickUpSocketName);
				HeldActor = HitActor;
				bIsHolding = true;
				return;
			}

			if (Tag.ToString() == "PickUp_Small")
			{
				UStaticMeshComponent* HitComponent = HitActor->FindComponentByClass<UStaticMeshComponent>();
				if (HitComponent)
				{
					HitComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
					HitComponent->SetSimulatePhysics(false);
				}
				HitActor->AttachToComponent(GrabMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SmallPickUpSocketName);
				HeldActor = HitActor;
				bIsHolding = true;
				bIsHoldingSmall = true;

				return;
			}
			else if (Tag.ToString() == "PickUp_Big")
			{
				UStaticMeshComponent* HitComponent = HitActor->FindComponentByClass<UStaticMeshComponent>();
				if (HitComponent)
				{
					HitComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
					HitComponent->SetSimulatePhysics(false);
				}
				HitActor->AttachToComponent(GrabMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, BigPickUpSocketName);
				HeldActor = HitActor;
				bIsHolding = true;
				return;
			}
			else if (Tag.ToString() == "Door")
			{
				TArray<UActorComponent*> DoorComponents;
				HitActor->GetComponents(DoorComponents);
				UDoorComponent* DoorActorComp = HitActor->GetComponentByClass<UDoorComponent>();
				bIsOpening = DoorActorComp->bIsOpened;
				UStaticMeshComponent* DoorMesh = nullptr;
				for (UActorComponent* Comp : DoorComponents)
				{
					UStaticMeshComponent* DoorStaticMeshComponent = Cast<UStaticMeshComponent>(Comp);
					if (DoorStaticMeshComponent && DoorStaticMeshComponent->ComponentHasTag(FName("RotatorDoorMesh")))
					{
						DoorMesh = DoorStaticMeshComponent;
						break;
					}
				}
				if (DoorMesh)
				{
					if (bIsOpening)
					{
						FRotator NewRotation = DoorMesh->GetComponentRotation();
						NewRotation.Yaw = 0;
						DoorMesh->SetRelativeRotation(NewRotation);
						//bIsOpening = false;
						DoorActorComp->bIsOpened = false; //actor is closed
						bIsClosingDoor = true; // animation
						DoorMesh = nullptr;
						return;
					}
					else if (!bIsOpening)
					{
						FRotator NewRotation = DoorMesh->GetComponentRotation();
						NewRotation.Yaw = -90.0f;
						DoorMesh->SetRelativeRotation(NewRotation);
						//bIsOpening = true;
						DoorActorComp->bIsOpened = true; // actor is opened
						bIsOpeningDoor = true; //animation
						DoorMesh = nullptr;
						return;
					}
				}
				else if(!DoorMesh)
				{
					DoorHintUICreateAndRemove();
					return;
				}
			}
			//else if (HitActor && Tag.ToString() == "Locker")
			//{
			//	if (bIsHolding && HeldActor && HeldActor->ActorHasTag("Key"))
			//	{
			//		HitActor->Destroy();
			//		HeldActor->Destroy();
			//		HeldActor = nullptr;
			//		bIsHolding = false;
			//	}
			//	else
			//	{
			//		//Say Need to Key
			//	}

			//	return;
			//}
		}
	}
}

AActor* UInteractSystem::GetActorInView()
{
	//return nullptr;
	AActor* ResultHitActor = nullptr;
	// LinceTrace Start and End Loaction Calculate
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		FVector Start = CameraLocation;
		FVector End = Start + (CameraRotation.Vector() * TraceDistance);

		//Linetrace Result
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());


		//Linetrace
		bool bhit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.5f, 0, 0.1f);

		if (bhit)
		{
			ResultHitActor = HitResult.GetActor();
			/*if (ResultHitActor->Tags.Num()>0) {
				FString ActorTag;
				FName Tag;
				Tag = ResultHitActor->Tags[0];
				UE_LOG(LogTemp, Log, TEXT("Actor: %s"), *Tag.ToString());
			}*/
			UpdateOutlineEffect(ResultHitActor);
			return ResultHitActor;
		}
		else
		{
			UpdateOutlineEffect(nullptr);
			return nullptr;
		}
	}
	else
	{
		UpdateOutlineEffect(nullptr);
		return nullptr;
	}
}

void UInteractSystem::CloseDoorLockUI()
{
	if (DoorLockWidget)
	{
		DoorLockWidget->RemoveFromParent();
		DoorLockWidget = nullptr;

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
	}
}

void UInteractSystem::DetachHeldActor()
{
	if (bIsHolding && HeldActor)
	{
		UStaticMeshComponent* HeldComponent = HeldActor->FindComponentByClass<UStaticMeshComponent>();
		if (HeldComponent)
		{
			HeldComponent->SetSimulatePhysics(true);
			//HeldComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//HeldComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}
		HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HeldActor = nullptr;
		bIsHolding = false;
		bIsHoldingSmall = false;

		return;
	}
}

void UInteractSystem::DoorHintUICreateAndRemove()
{
	if (LockHintWidget)
	{
		LockHintWidget->RemoveFromParent();
		LockHintWidget = nullptr;
	}
	if (!DoorHintWidget && DoorHintWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		DoorHintWidget = CreateWidget<UUserWidget>(PlayerController, DoorHintWidgetClass);
		if (DoorHintWidget)
		{
			DoorHintWidget->AddToViewport();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
				{
					if (DoorHintWidget && DoorHintWidget->IsValidLowLevel())
					{
						DoorHintWidget->RemoveFromParent();
						DoorHintWidget = nullptr; // 포인터 초기화
					}
				}), 1.0f, false); // 1초 후 한 번만 실행
		}
	}
}

void UInteractSystem::LockHintUICreateAndRemove()
{
	if (DoorHintWidget)
	{
		DoorHintWidget->RemoveFromParent();
		DoorHintWidget = nullptr;
	}
	if (!LockHintWidget && LockHintWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		LockHintWidget = CreateWidget<UUserWidget>(PlayerController, LockHintWidgetClass);
		if (LockHintWidget)
		{
			LockHintWidget->AddToViewport();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
				{
					if (LockHintWidget && LockHintWidget->IsValidLowLevel())
					{
						LockHintWidget->RemoveFromParent();
						LockHintWidget = nullptr; // 포인터 초기화
					}
				}), 1.0f, false); // 1초 후 한 번만 실행
		}
	}
}

void UInteractSystem::UpdateOutlineEffect(AActor* TargetActor)
{
	//TargetActor를 들고 있거나, 이미 Outliner가 적용 되어있을 때
	if (TargetActor == HeldActor || CurrentOutlinerActor == TargetActor) return;


	if (CurrentOutlinerActor)
	{
		UStaticMeshComponent* MeshComp = CurrentOutlinerActor->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetRenderCustomDepth(false);
			MeshComp->SetCustomDepthStencilValue(0);

		}
	}

	if (TargetActor && TargetActor->Tags.Num() > 0)
	{
		UStaticMeshComponent* MeshComp = TargetActor->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetRenderCustomDepth(true);
			MeshComp->SetCustomDepthStencilValue(1);
		}
	}

	CurrentOutlinerActor = TargetActor;
}

