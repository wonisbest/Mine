// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"

AMyAIController::AMyAIController()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	if (PawnSensingComponent)
	{
		PawnSensingComponent->PrimaryComponentTick.bCanEverTick = true;
		PawnSensingComponent->bEnableSensingUpdates = true;
		PawnSensingComponent->bAutoActivate = true;
	}
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	//NPC 시작 위치 저장
	ControlledCharacter = Cast<ACharacter>(GetPawn());
	NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	LevelScript = GetWorld()->GetLevelScriptActor();
	if (ControlledCharacter)
	{
		TargetLocation1 = ControlledCharacter->GetActorLocation();
		TargetLocation2 = TargetLocation2 + FVector(-3300.0f, 0.0f, 0.0f);
	}

	if (PawnSensingComponent)
	{
		PawnSensingComponent->SetPeripheralVisionAngle(120.0f);//시야각 설정
		PawnSensingComponent->SightRadius = 1500.0f; //감지 범위 설정
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMyAIController::OnSeePawn);//Player를 보면 ChasePlayer 실행
		//GetWorld()->GetTimerManager().SetTimer(DebugDrawTimer, this, &AMyAIController::DrawDebugSensing, 0.1f, true);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PawnSensingComponent is nullptr"));
	}
}

void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* MyPawn = GetPawn();
	//ACharacter* ControlledCharacter = Cast<ACharacter>(MyPawn);
	if (!ControlledCharacter)
	{
		return;
	}

	if (bisChasing && TargetPlayer)
	{
		float DistanceToplayer = FVector::Dist(ControlledCharacter->GetActorLocation(), TargetPlayer->GetActorLocation());

		bisPlayerOnNav = NavigationSystem->ProjectPointToNavigation(TargetPlayer->GetActorLocation(), ProjectedLocation);

		if (!bisPlayerOnNav)
		{

			bisChasing = false;
			TargetPlayer = nullptr;
			//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyAIController::MoveToNextTarget, 2.0f, false);
			MoveToNextTarget();
		}

		if (DistanceToplayer < 200.0f)
		{
			AttackPlayer();
		}

		else if (DistanceToplayer > 1500.0f)
		{
			bisChasing = false;
			TargetPlayer = nullptr;
			MoveToNextTarget();
		}
	}
	
	
	
	if (ControlledCharacter)
	{
		float DistanceToTarget = FVector::Dist(ControlledCharacter->GetActorLocation(), bisMovingToTarget1 ? TargetLocation1 : TargetLocation2);
		if (DistanceToTarget < 150.0f)
		{
			bisMovingToTarget1 = !bisMovingToTarget1;
			//MoveToNextTarget();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyAIController::MoveToNextTarget, 0.5f, false);
		}
	}
}

ALevelScriptActor* AMyAIController::GetLevelBP()
{
	if (LevelScript)
	{
		
		return LevelScript;
	}
	else
	{
		return nullptr;
	}
}

void AMyAIController::MoveToNextTarget()
{
	if (ControlledCharacter && ControlledCharacter->GetCharacterMovement())
	{
		if (!bisChasing)
		{
			ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		}
	}
	FVector NextTarget = bisMovingToTarget1 ? TargetLocation1 : TargetLocation2;
	MoveToLocation(NextTarget);
}

void AMyAIController::ChasePlayer(APawn* PlayerPawn)
{
	
	if (!PlayerPawn)
	{
		return;
	}
	if (!bisChasing)
	{
		TargetPlayer = PlayerPawn;
		bisChasing = true;
		MoveToActor(PlayerPawn, 100.0f);
	}
}

void AMyAIController::AttackPlayer()
{
	if (TargetPlayer && !bisAttacking)
	{
		bisAttacking = true;

		if (APlayerController* PlayerController = Cast<APlayerController>(TargetPlayer->GetController()))
		{
			PlayerController->DisableInput(PlayerController);
			PlayerController->SetIgnoreLookInput(true);
			PlayerController->SetIgnoreMoveInput(true);

			FVector NPCLocation = GetPawn()->GetActorLocation();
			FVector PlayerLoaction = TargetPlayer->GetActorLocation();
			FRotator LookAtRotation = (NPCLocation - PlayerLoaction).Rotation();
			PlayerController->SetControlRotation(LookAtRotation);

			if (!GameOverkWidget && GameOverWidgetClass)
			{
				if (PlayerController)
				{
					GameOverkWidget = CreateWidget<UUserWidget>(PlayerController, GameOverWidgetClass);
					if (GameOverkWidget)
					{
						//GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &AMyAIController::DoNothing, 1.0f, false);
						GameOverkWidget->AddToViewport();
						FInputModeUIOnly InputMode;
						PlayerController->SetInputMode(InputMode);
						PlayerController->bShowMouseCursor = true;
					}
				}
			}
		}
	}
}

void AMyAIController::DrawDebugSensing()
{
	if (PawnSensingComponent && GetPawn())
	{
		DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), PawnSensingComponent->SightRadius, 12, FColor::Red, false, 0.1f);
	}
}

void AMyAIController::OnSeePawn(APawn* PlayerPawn)
{
	//bisPlayerOnNav = NavigationSystem->ProjectPointToNavigation(TargetPlayer->GetActorLocation(), ProjectedLocation);
	if (!PlayerPawn)
	{
		return;
	}
	if (!bisChasing)
	{
		TargetPlayer = PlayerPawn;
		bisChasing = true;
		if (ControlledCharacter && ControlledCharacter->GetCharacterMovement())
		{
			if (bisChasing)
			{
				ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = 450.0f;
			}
		}
		MoveToActor(PlayerPawn, 100.0f);
	}
}

bool AMyAIController::GetIsAttacking()
{
	return bisAttacking;
}

bool AMyAIController::GetIsChasing()
{
	return bisChasing;
}

void AMyAIController::DoNothing()
{
}

void AMyAIController::FindLevelAudioComponent()
{

}
