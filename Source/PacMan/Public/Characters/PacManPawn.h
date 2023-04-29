// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridPawn.h"
#include "Grid/GridGenerator.h"
#include "Grid/GridPlayerController.h"
#include "Grid/GridBaseNode.h"
#include "Grid/PacManGameInstance.h"
#include "Characters/PhantomPawn.h"
#include "PacManPawn.generated.h"


UCLASS()
class PACMAN_API APacManPawn : public AGridPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacManPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetVerticalInput(float AxisValue) override;
	virtual void SetHorizontalInput(float AxisValue) override;
	// called on left mouse click (binding)
	virtual	void OnClick() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPacManGameInstance* GameInstance;
	
	UFUNCTION(Blueprintcallable)
	void Reset_LastDirection();

protected:
	virtual void BeginPlay() override;
	virtual void HandleMovement() override;
	virtual void SetTargetNode(AGridBaseNode* Node) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
