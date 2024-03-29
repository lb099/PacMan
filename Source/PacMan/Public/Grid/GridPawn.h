// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridBaseNode.h"
#include "Grid/GridGenerator.h"
#include "GameFramework/Pawn.h"
#include "Grid/PacManGameInstance.h"
#include "Components/BoxComponent.h"
#include "GridPawn.generated.h"

UENUM()
enum EPhantomName {
	Blinky,
	Clyde,
	Inky,
	Pinky,
	Vuoto

};

UCLASS()
class PACMAN_API AGridPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGridPawn();
	virtual void SetVerticalInput(float AxisValue);
	virtual void SetHorizontalInput(float AxisValue);

	UFUNCTION(BlueprintCallable)
		void SetNextNodeByDir(FVector InputDir, bool ForceLast = false);

	FVector GetLastValidDirection() const;

	UFUNCTION(BlueprintCallable)
		AGridBaseNode* GetLastNode() const;
	UFUNCTION(BlueprintCallable)
		AGridBaseNode* GetTargetNode() const;
	UFUNCTION(BlueprintCallable)
		FVector2D GetLastNodeCoords() const;
	UFUNCTION(BlueprintCallable)
		FVector2D GetTargetNodeCoords() const;

	UPacManGameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastValidInputDirection;

	void SetLastValidDirection(FVector Dir);

	UPROPERTY(EditAnywhere, Category = "Movement")
		float CurrentMovementSpeed = 100.0f;

		float FrightenedMovementSpeed = 80.0f;
		float HomeMovementSpeed = 1000.0f;

		float TeleportMovementSpeed = 10000000000.0f;
		float DeadMovementSpeed = 10000000000.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float NormalMovementSpeed = 100.0f;
	UPROPERTY(EditAnywhere)
		float AcceptedDistance = 4.f;

	UPROPERTY(VisibleAnywhere)
		FVector2D CurrentGridCoords;

	UPROPERTY(VisibleAnywhere)
		class AGridGameMode* GameMode;

	UPROPERTY(VisibleAnywhere)
		AGridGenerator* TheGridGen;

	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		AGridBaseNode* NextNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		AGridBaseNode* TargetNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		AGridBaseNode* LastNode;

	virtual void HandleMovement();
	virtual void OnNodeReached();
	void MoveToCurrentTargetNode();
	virtual void SetTargetNode(AGridBaseNode* Node);
	void SetNextNode(AGridBaseNode* Node);
	void SetNodeGeneric(const FVector Dir);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;

	bool bTeleport = false;
	bool bIsDead = false;
	UPROPERTY(VisibleAnywhere)
	bool bCanMove = true;
	

	void Set_Teleport(bool pTeleport) {
		bTeleport = pTeleport;
	}

	bool Check_Teleport() {
		return bTeleport;
	}

	void Set_Dead(bool pIsDead) {
		bIsDead = pIsDead;
	}
	bool Check_Dead() {
		return bIsDead;
	}

	UFUNCTION(Blueprintcallable)
	void Set_CanMove(bool pCanMove) {
		bCanMove = pCanMove;
	}
	UFUNCTION(Blueprintcallable)
	bool Check_CanMove() {
		return bCanMove;
	}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// called on left mouse click (binding)
	virtual void OnClick();

	UPROPERTY(EditAnywhere)
		TEnumAsByte <EPhantomName> EPhantomName = Vuoto;


};
