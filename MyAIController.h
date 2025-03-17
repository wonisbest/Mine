// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h."
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "NavigationSystem.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MINE_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI")
	bool bisChasing = false;
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bisAttacking = false;
	UFUNCTION(BlueprintCallable, Category="Level")
	ALevelScriptActor* GetLevelBP();
private:
	FVector TargetLocation1;
	FVector TargetLocation2;
	bool bisMovingToTarget1 = true;

	FTimerHandle TimerHandle;
	FTimerHandle DebugDrawTimer; 
	UNavigationSystemV1* NavigationSystem;
	FNavLocation ProjectedLocation;
	bool bisPlayerOnNav;
	void MoveToNextTarget();
	void ChasePlayer(APawn* PlayerPawn);
	void AttackPlayer();

	UPROPERTY(VisibleAnywhere,Category="AI")
	UPawnSensingComponent* PawnSensingComponent;

	APawn* TargetPlayer = nullptr;

	void DrawDebugSensing();

	UFUNCTION()
	void OnSeePawn(APawn* PlayerPawn);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool GetIsAttacking();
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool GetIsChasing();

	ACharacter* ControlledCharacter;
	UUserWidget* GameOverkWidget;
	ALevelScriptActor* LevelScript;

	void DoNothing();
	void FindLevelAudioComponent();
};
